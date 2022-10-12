/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qmath.h>

#include "qtextureglyphcache_p.h"

#include "private/qnumeric_p.h"
#include "private/qnativeimage_p.h"
#include "private/qfontengine_ft_p.h"

QT_BEGIN_NAMESPACE

// #define CACHE_DEBUG

// returns the highest number closest to v, which is a power of 2
// NB! assumes 32 bit ints
static inline int qt_next_power_of_two(int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	++v;
	return v;
}

#if 0 //[YY. zyf del]

bool QTextureGlyphCache::populate(QFontEngine *fontEngine, int numGlyphs, const glyph_t *glyphs,
								  const QFixedPoint *)
{
#ifdef CACHE_DEBUG
	printf("Populating with %d glyphs\n", numGlyphs);
	qDebug() << " -> current transformation: " << m_transform;
#endif

	m_current_fontengine = fontEngine;
	const int margin = glyphMargin();
	const int paddingDoubled = glyphPadding() * 2;

	QHash<glyph_t, Coord> listItemCoordinates;
	int rowHeight = 0;

	// check each glyph for its metrics and get the required rowHeight.
	for (int i=0; i < numGlyphs; ++i) {
		const glyph_t glyph = glyphs[i];
		if (coords.contains(glyph))
			continue;
		if (listItemCoordinates.contains(glyph))
			continue;
		glyph_metrics_t metrics = fontEngine->boundingBox(glyph, m_transform);

#ifdef CACHE_DEBUG
		printf("(%4x): w=%.2f, h=%.2f, xoff=%.2f, yoff=%.2f, x=%.2f, y=%.2f\n",
			glyph,
			metrics.width.toReal(),
			metrics.height.toReal(),
			metrics.xoff.toReal(),
			metrics.yoff.toReal(),
			metrics.x.toReal(),
			metrics.y.toReal());
#endif
		int glyph_width = metrics.width.ceil().toInt();
		int glyph_height = metrics.height.ceil().toInt();
		if (glyph_height == 0 || glyph_width == 0)
			continue;
		glyph_width += margin * 2 + 4;
		glyph_height += margin * 2 + 4;
		// align to 8-bit boundary
		if (m_type == QFontEngineGlyphCache::Raster_Mono)
			glyph_width = (glyph_width+7)&~7;

		Coord c = { 0, 0, // will be filled in later
			glyph_width,
			glyph_height, // texture coords
			metrics.x.round().truncate(),
			-metrics.y.truncate() }; // baseline for horizontal scripts

		listItemCoordinates.insert(glyph, c);
		rowHeight = qMax(rowHeight, glyph_height);
	}
	if (listItemCoordinates.isEmpty())
        return true;

	rowHeight += margin * 2 + paddingDoubled;
	if (isNull())
		createCache(QT_DEFAULT_TEXTURE_GLYPH_CACHE_WIDTH, qt_next_power_of_two(rowHeight));

	// now actually use the coords and paint the wanted glyps into cache.
	QHash<glyph_t, Coord>::iterator iter = listItemCoordinates.begin();
	while (iter != listItemCoordinates.end()) {
		Coord c = iter.value();

		m_currentRowHeight = qMax(m_currentRowHeight, c.h + margin * 2);

		if (m_cx + c.w > m_w) {
			int new_width = m_w*2;
			while (new_width < m_cx + c.w)
				new_width *= 2;
			if (new_width <= maxTextureWidth()) {
				resizeTextureData(new_width, m_h);
				m_w = new_width;
			} else {
				// no room on the current line, start new glyph strip
				m_cx = 0;
				m_cy += m_currentRowHeight + paddingDoubled;
                m_currentRowHeight = c.h + margin * 2; // New row
			}
		}
		if (m_cy + c.h > m_h) {
			int new_height = m_h*2;
			while (new_height < m_cy + c.h)
				new_height *= 2;

            if (maxTextureHeight() > 0 && new_height > maxTextureHeight()) {
                // We can't make a new texture of the required size, so
                // bail out
                return false;
            }

			// if no room in the current texture - realloc a larger texture
			resizeTextureData(m_w, new_height);
			m_h = new_height;
		}

		c.x = m_cx;
		c.y = m_cy;

		fillTexture(c, iter.key());
		coords.insert(iter.key(), c);

		m_cx += c.w + paddingDoubled;
		++iter;
	}

    return true;
}

QImage QTextureGlyphCache::textureMapForGlyph(glyph_t g) const
{
#if defined(Q_WS_X11)
    if (m_type != Raster_RGBMask && m_transform.type() > QTransform::TxTranslate && m_current_fontengine->type() == QFontEngine::Freetype) {
		QFontEngineFT::GlyphFormat format = QFontEngineFT::Format_None;
		QImage::Format imageFormat = QImage::Format_Invalid;
		switch (m_type) {
		case Raster_A8:
			format = QFontEngineFT::Format_A8;
			imageFormat = QImage::Format_Indexed8;
			break;
		case Raster_Mono:
			format = QFontEngineFT::Format_Mono;
			imageFormat = QImage::Format_Mono;
			break;
		};

		QFontEngineFT *ft = static_cast<QFontEngineFT*> (m_current_fontengine);
		QFontEngineFT::QGlyphSet *gset = ft->loadTransformedGlyphSet(m_transform);

		if (gset && ft->loadGlyphs(gset, &g, 1, format)) {
			QFontEngineFT::Glyph *glyph = gset->getGlyph(g);
			const int bytesPerLine = (format == QFontEngineFT::Format_Mono ? ((glyph->width + 31) & ~31) >> 3
				: (glyph->width + 3) & ~3);
			return QImage(glyph->data, glyph->width, glyph->height, bytesPerLine, imageFormat);
		}
	} else
#endif
		if (m_type == QFontEngineGlyphCache::Raster_RGBMask)
			return m_current_fontengine->alphaRGBMapForGlyph(g, glyphMargin(), m_transform);
		else
			return m_current_fontengine->alphaMapForGlyph(g, m_transform);

	return QImage();
}

/************************************************************************
* QImageTextureGlyphCache
*/

void QImageTextureGlyphCache::resizeTextureData(int width, int height)
{
	m_image = m_image.copy(0, 0, width, height);
}

void QImageTextureGlyphCache::createTextureData(int width, int height)
{
	switch (m_type) {
	case QFontEngineGlyphCache::Raster_Mono:
		m_image = QImage(width, height, QImage::Format_Mono);
		break;
	case QFontEngineGlyphCache::Raster_A8: {
		m_image = QImage(width, height, QImage::Format_Indexed8);
		m_image.fill(0);
		QVector<QRgb> colors(256);
		QRgb *it = colors.data();
		for (int i=0; i<256; ++i, ++it)
			*it = 0xff000000 | i | (i<<8) | (i<<16);
		m_image.setColorTable(colors);
		break;   }
	case QFontEngineGlyphCache::Raster_RGBMask:
		m_image = QImage(width, height, QImage::Format_RGB32);
		break;
	}
}

int QImageTextureGlyphCache::glyphMargin() const
{
#if defined(Q_WS_MAC) && defined(QT_MAC_USE_COCOA)
	return 0;
#else
	return m_type == QFontEngineGlyphCache::Raster_RGBMask ? 2 : 0;
#endif
}

void QImageTextureGlyphCache::fillTexture(const Coord &c, glyph_t g)
{
	QImage mask = textureMapForGlyph(g);

#ifdef CACHE_DEBUG
	printf("fillTexture of %dx%d at %d,%d in the cache of %dx%d\n", c.w, c.h, c.x, c.y, m_image.width(), m_image.height());
	if (mask.width() > c.w || mask.height() > c.h) {
		printf("   ERROR; mask is bigger than reserved space! %dx%d instead of %dx%d\n", mask.width(), mask.height(), c.w,c.h);
		return;
	}
#endif

	if (m_type == QFontEngineGlyphCache::Raster_RGBMask) {
        QImage ref(m_image.bits() + (c.x * 4 + c.y * m_image.bytesPerLine()),
                   qMax(mask.width(), c.w), qMax(mask.height(), c.h), m_image.bytesPerLine(),
                   m_image.format());
        QPainter p(&ref);
		p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, c.w, c.h, QColor(0,0,0,0)); // TODO optimize this
        p.drawImage(0, 0, mask);
		p.end();
	} else if (m_type == QFontEngineGlyphCache::Raster_Mono) {
		if (mask.depth() > 1) {
			// TODO optimize this
			mask = mask.alphaChannel();
			mask.invertPixels();
			mask = mask.convertToFormat(QImage::Format_Mono);
		}

		int mw = qMin(mask.width(), c.w);
		int mh = qMin(mask.height(), c.h);
		uchar *d = m_image.bits();
		int dbpl = m_image.bytesPerLine();

		for (int y = 0; y < c.h; ++y) {
			uchar *dest = d + (c.y + y) *dbpl + c.x/8;

			if (y < mh) {
				uchar *src = mask.scanLine(y);
				for (int x = 0; x < c.w/8; ++x) {
					if (x < (mw+7)/8)
						dest[x] = src[x];
					else
						dest[x] = 0;
				}
			} else {
				for (int x = 0; x < c.w/8; ++x)
					dest[x] = 0;
			}
		}
	} else { // A8
		int mw = qMin(mask.width(), c.w);
		int mh = qMin(mask.height(), c.h);
		uchar *d = m_image.bits();
		int dbpl = m_image.bytesPerLine();

		if (mask.depth() == 1) {
			for (int y = 0; y < c.h; ++y) {
				uchar *dest = d + (c.y + y) *dbpl + c.x;
				if (y < mh) {
					uchar *src = (uchar *) mask.scanLine(y);
					for (int x = 0; x < c.w; ++x) {
						if (x < mw)
							dest[x] = (src[x >> 3] & (1 << (7 - (x & 7)))) > 0 ? 255 : 0;
					}
				}
			}
		} else if (mask.depth() == 8) {
			for (int y = 0; y < c.h; ++y) {
				uchar *dest = d + (c.y + y) *dbpl + c.x;
				if (y < mh) {
					uchar *src = (uchar *) mask.scanLine(y);
					for (int x = 0; x < c.w; ++x) {
						if (x < mw)
							dest[x] = src[x];
					}
				}
			}
		}
	}

#ifdef CACHE_DEBUG
	//     QPainter p(&m_image);
	//     p.drawLine(
	QPoint base(c.x + glyphMargin(), c.y + glyphMargin() + c.baseLineY-1);
	if (m_image.rect().contains(base))
		m_image.setPixel(base, 255);
	m_image.save(QString::fromLatin1("cache-%1.png").arg(int(this)));
#endif
}

#else //[YY. zyf add]

/************************************************************************
* QTextureGlyphCache
*/

QTextureGlyphCache* QTextureGlyphCache::createGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
{
	if (type == QFontEngineGlyphCache::Raster_RGBMask)
		return new QCompressMemTextureGlyphCache(type, matrix);
	else
		return new QImageTextureGlyphCache(type, matrix);	
}

void QTextureGlyphCache::populate(QFontEngine *fontEngine, int numGlyphs, const glyph_t *glyphs, 
								  const QFixedPoint *)
{
	m_current_fontengine = fontEngine;
	
	for (int i = 0; i < numGlyphs; ++i)
	{
		populateSingleGlyph(glyphs[i]);
	}
}

QImage QTextureGlyphCache::textureMapForGlyph(glyph_t g) const
{
#if defined(Q_WS_X11)
	if (m_transform.type() > QTransform::TxTranslate) {
		QFontEngineFT::GlyphFormat format = QFontEngineFT::Format_None;
		QImage::Format imageFormat = QImage::Format_Invalid;
		switch (m_type) {
		case Raster_RGBMask:
			format = QFontEngineFT::Format_A32;
			imageFormat = QImage::Format_RGB32;
			break;
		case Raster_A8:
			format = QFontEngineFT::Format_A8;
			imageFormat = QImage::Format_Indexed8;
			break;
		case Raster_Mono:
			format = QFontEngineFT::Format_Mono;
			imageFormat = QImage::Format_Mono;
			break;
		};

		QFontEngineFT *ft = static_cast<QFontEngineFT*> (m_current_fontengine);
		QFontEngineFT::QGlyphSet *gset = ft->loadTransformedGlyphSet(m_transform);

		if (gset && ft->loadGlyphs(gset, &g, 1, format)) {
			QFontEngineFT::Glyph *glyph = gset->getGlyph(g);
			const int bytesPerLine = (format == QFontEngineFT::Format_Mono ? ((glyph->width + 31) & ~31) >> 3
				: (glyph->width + 3) & ~3);
			return QImage(glyph->data, glyph->width, glyph->height, bytesPerLine, imageFormat);
		}
	} else
#endif
		if (m_type == QFontEngineGlyphCache::Raster_RGBMask)
			return m_current_fontengine->alphaRGBMapForGlyph(g, glyphMargin(), m_transform);
		else
			return m_current_fontengine->alphaMapForGlyph(g, m_transform);

	return QImage();
}

bool QTextureGlyphCache::populateSingleGlyph(glyph_t glyph)
{
	const int margin = glyphMargin();
	if (!findTexture(glyph))
	{
		glyph_metrics_t metrics = m_current_fontengine->boundingBox(glyph, m_transform);
		int glyph_width = metrics.width.ceil().toInt();
		int glyph_height = metrics.height.ceil().toInt();
		if (glyph_height == 0 || glyph_width == 0)
			return false;
		glyph_width += margin * 2 + 4;
		glyph_height += margin * 2 + 4;
		// align to 8-bit boundary
		if (m_type == QFontEngineGlyphCache::Raster_Mono)
			glyph_width = (glyph_width+7)&~7;

		fillTexture(glyph, glyph_width, glyph_height, 
			metrics.x.round().truncate(), 
			-metrics.y.truncate());
	}
	return true;
}

/************************************************************************
* QImageTextureGlyphCache
*/

bool QImageTextureGlyphCache::findTexture(glyph_t g)
{
	return coords.contains(g);
}

void QImageTextureGlyphCache::fillTexture(glyph_t g, int w, int h, int baseLineX, int baseLineY)
{
	int rowHeight = h;
	rowHeight += glyphMargin() * 2 + glyphPadding() * 2;

	if (isNull())
		createTextureData(QT_DEFAULT_TEXTURE_GLYPH_CACHE_WIDTH, qt_next_power_of_two(rowHeight));

	m_currentRowHeight = qMax(m_currentRowHeight, h + glyphMargin() * 2);

	if (m_cx + w > m_w) {
		int new_width = m_w*2;
		while (new_width < m_cx + w)
			new_width *= 2;
		if (new_width <= maxTextureWidth()) {
			resizeTextureData(new_width, m_h);
			m_w = new_width;
		} else {
			// no room on the current line, start new glyph strip
			m_cx = 0;
			m_cy += m_currentRowHeight + glyphPadding() * 2;
			m_currentRowHeight = 0; // New row
		}
	}
	if (m_cy + h > m_h) {
		int new_height = m_h*2;
		while (new_height < m_cy + h)
			new_height *= 2;
		// if no room in the current texture - realloc a larger texture
		resizeTextureData(m_w, new_height);
		m_h = new_height;
	}

	Coord c = { 0 };
	c.x = m_cx;
	c.y = m_cy;
	c.w = w;
	c.h = h;
	c.baseLineX = baseLineX;
	c.baseLineY = baseLineY;

	fillTextureData(c, g);
	coords.insert(g, c);

	m_cx += c.w + glyphPadding() * 2;
}

int QImageTextureGlyphCache::glyphMargin() const
{
#if defined(Q_WS_MAC) && defined(QT_MAC_USE_COCOA)
	return 0;
#else
	return m_type == QFontEngineGlyphCache::Raster_RGBMask ? 2 : 0;
#endif
}

void QImageTextureGlyphCache::resizeTextureData(int width, int height)
{
	m_image = m_image.copy(0, 0, width, height);
}

void QImageTextureGlyphCache::createTextureData(int width, int height)
{
	switch (m_type) {
	case QFontEngineGlyphCache::Raster_Mono:
		m_image = QImage(width, height, QImage::Format_Mono);
		break;
	case QFontEngineGlyphCache::Raster_A8: {
		m_image = QImage(width, height, QImage::Format_Indexed8);
		m_image.fill(0);
		QVector<QRgb> colors(256);
		QRgb *it = colors.data();
		for (int i=0; i<256; ++i, ++it)
			*it = 0xff000000 | i | (i<<8) | (i<<16);
		m_image.setColorTable(colors);
		break;   }
	case QFontEngineGlyphCache::Raster_RGBMask:
		m_image = QImage(width, height, QImage::Format_RGB32);
		break;
	}
	m_w = width;
	m_h = height;
}

void QImageTextureGlyphCache::fillTextureData(const Coord &c, glyph_t g)
{
	QImage mask = textureMapForGlyph(g);

#ifdef CACHE_DEBUG
	printf("fillTexture of %dx%d at %d,%d in the cache of %dx%d\n", c.w, c.h, c.x, c.y, m_image.width(), m_image.height());
	if (mask.width() > c.w || mask.height() > c.h) {
		printf("   ERROR; mask is bigger than reserved space! %dx%d instead of %dx%d\n", mask.width(), mask.height(), c.w,c.h);
		return;
	}
#endif

	if (m_type == QFontEngineGlyphCache::Raster_RGBMask) {
		QPainter p(&m_image);
		p.setCompositionMode(QPainter::CompositionMode_Source);
		p.fillRect(c.x, c.y, c.w, c.h, QColor(0,0,0,0)); // TODO optimize this

		QSize size = mask.size();
		p.drawImage(c.x, c.y, mask);
		p.end();
	} else if (m_type == QFontEngineGlyphCache::Raster_Mono) {
		if (mask.depth() > 1) {
			// TODO optimize this
			mask = mask.alphaChannel();
			mask.invertPixels();
			mask = mask.convertToFormat(QImage::Format_Mono);
		}

		int mw = qMin(mask.width(), c.w);
		int mh = qMin(mask.height(), c.h);
		uchar *d = m_image.bits();
		int dbpl = m_image.bytesPerLine();

		for (int y = 0; y < c.h; ++y) {
			uchar *dest = d + (c.y + y) *dbpl + c.x/8;

			if (y < mh) {
				uchar *src = mask.scanLine(y);
				for (int x = 0; x < c.w/8; ++x) {
					if (x < (mw+7)/8)
						dest[x] = src[x];
					else
						dest[x] = 0;
				}
			} else {
				for (int x = 0; x < c.w/8; ++x)
					dest[x] = 0;
			}
		}
	} else { // A8
		int mw = qMin(mask.width(), c.w);
		int mh = qMin(mask.height(), c.h);
		uchar *d = m_image.bits();
		int dbpl = m_image.bytesPerLine();

		if (mask.depth() == 1) {
			for (int y = 0; y < c.h; ++y) {
				uchar *dest = d + (c.y + y) *dbpl + c.x;
				if (y < mh) {
					uchar *src = (uchar *) mask.scanLine(y);
					for (int x = 0; x < c.w; ++x) {
						if (x < mw)
							dest[x] = (src[x >> 3] & (1 << (7 - (x & 7)))) > 0 ? 255 : 0;
					}
				}
			}
		} else if (mask.depth() == 8) {
			for (int y = 0; y < c.h; ++y) {
				uchar *dest = d + (c.y + y) *dbpl + c.x;
				if (y < mh) {
					uchar *src = (uchar *) mask.scanLine(y);
					for (int x = 0; x < c.w; ++x) {
						if (x < mw)
							dest[x] = src[x];
					}
				}
			}
		}
	}
}

bool QImageTextureGlyphCache::getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, int* bpl, int* depth, int* rx, int* ry, int* w, int* h)
{
	static const qreal aliasedCoordinateDelta = 0.5 - 0.015625;
	static const QFixed offs = QFixed::fromReal(aliasedCoordinateDelta);

	QHash<glyph_t, Coord>::iterator itGlyph = coords.find(g);
	if (itGlyph == coords.end()) {
		return false;
	}
	const Coord& c = itGlyph.value();
	*bpl = m_image.bytesPerLine();
	*depth = m_image.depth();

	int rightShift = 0;
	int leftShift = 0;
	if (*depth == 32)
		leftShift = 2; // multiply by 4
	else if (*depth == 1)
		rightShift = 3; // divide by 8
	*rx = qFloor(position->x + offs) + c.baseLineX - glyphMargin();
	*ry = qFloor(position->y + offs) - c.baseLineY - glyphMargin();
	*w = c.w;
	*h = c.h;

	const uchar *bits = m_image.bits();
	*data = bits + ((c.x << leftShift) >> rightShift) + c.y * (*bpl);
	return true;
}

/************************************************************************
* QMemTextureGlyphCache
*/


inline
void* devdiv_malloc_protected(size_t size)
{
	int page_num = (int) (size / 0x1000 + 1);
	size_t offset = page_num * 0x1000 - size;
	void* p = ::VirtualAlloc(NULL, page_num*0x1000+1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (NULL == p)
	{
		//        ASSERT(FALSE);
		return NULL; // 内存用尽
	}
	void* pchecker = (char*)p + page_num * 0x1000;
	DWORD old_value;
	::VirtualProtect(pchecker, 0x1000, PAGE_NOACCESS, &old_value);
	return (char*)p + offset;
}

inline
void devdiv_free_protected(void* p)
{
	//找到VirtualAlloc时候的起始地址
	p = (char*)p - (size_t)p % 0x1000;
	::VirtualFree(p, 0, MEM_RELEASE);
}

#ifdef CACHE_DEBUG
	#define MEM_TRUNK_MALLOC(size) devdiv_malloc_protected(size)
	#define MEM_TRUNK_FREE(p) devdiv_free_protected(p)
#else
	#define MEM_TRUNK_MALLOC(size) ::malloc(size)
	#define MEM_TRUNK_FREE(p) ::free(p)
#endif

#define MIN_MEM_CHRUNK_SIZE (4 * 1024)
#define MAX_MEM_CHRUNK_SIZE (32 * 1024)
#define MAX_MEM_CHRUNK_COUNT 10

QMemTextureGlyphCache::~QMemTextureGlyphCache()
{
	foreach(MemChrunk* chrunk, memChurnkList)
	{
		MEM_TRUNK_FREE(chrunk);
	}
	memChurnkList.clear();
	glyphDatas.clear();
}

bool QMemTextureGlyphCache::findTexture(glyph_t g)
{
	return glyphDatas.contains(g);
}

void QMemTextureGlyphCache::fillTexture(glyph_t g, int w, int h, int baseLineX, int baseLineY)
{
	int requestLen = w * h * (m_depth >> 3);
	MemChrunk* memChurnk = NULL;
	getMemChrunk(requestLen, memChurnk);

	GlyphData glyphData = { 0 };
	glyphData.w = w;
	glyphData.h = h;
	glyphData.baseLineX = baseLineX;
	glyphData.baseLineY = baseLineY;
	glyphData.data = memChurnk;
	glyphData.data_offset = memChurnk->chrunkSize - memChurnk->spareBytes;
	glyphData.data_len = requestLen;

	fillTextureData(glyphData, g);
	glyphDatas.insert(g, glyphData);
}

void QMemTextureGlyphCache::getMemChrunk(int requestLen, MemChrunk*& memChrunk)
{
	Q_ASSERT(requestLen < MAX_MEM_CHRUNK_SIZE);

	if (memChurnkList.empty()) //无mem chrunk存储
	{
		int chrunkSize = MIN_MEM_CHRUNK_SIZE;
		if (chrunkSize < requestLen)
			chrunkSize = MAX_MEM_CHRUNK_SIZE;

		memChrunk = (MemChrunk*)MEM_TRUNK_MALLOC(sizeof(MemChrunk) + (chrunkSize) - 1);
		memChrunk->timeStamp = QTime::currentTime();
		memChrunk->spareBytes = chrunkSize;
		memChrunk->chrunkSize = chrunkSize;
		::memset(memChrunk->data, 0, chrunkSize);
		memChurnkList.push_back(memChrunk);
	}
	else if (memChurnkList.back()->spareBytes >= requestLen) //最后一个chrunk块剩余空间可以满足要求
	{
		memChrunk = memChurnkList.back();
	}
	else if (memChurnkList.size() < MAX_MEM_CHRUNK_COUNT) //未达到chrunk存储块数限制，那么申请一个新的chrunk
	{
		int chrunkSize = memChurnkList.back()->chrunkSize;
		if (chrunkSize < MAX_MEM_CHRUNK_SIZE)
			chrunkSize *= 2;
		if (chrunkSize < requestLen)
			chrunkSize = MAX_MEM_CHRUNK_SIZE;

		memChrunk = (MemChrunk*)MEM_TRUNK_MALLOC(sizeof(MemChrunk) + (chrunkSize) - 1);
		memChrunk->timeStamp = QTime::currentTime();
		memChrunk->spareBytes = chrunkSize;
		memChrunk->chrunkSize = chrunkSize;
		::memset(memChrunk->data, 0, chrunkSize);
		memChurnkList.push_back(memChrunk);
	}
	else //已经达到chrunk存储块数限定值，做两件事情：1、遍历chrunk寻找空闲空间。 2、如果无法满足，那么淘汰最近未被使用的chrunk 
	{
		QLinkedList<MemChrunk*>::iterator  itLastTimeStampChrunk = memChurnkList.end() - 1;

		for (QLinkedList<MemChrunk*>::iterator itChrunk = memChurnkList.begin();
			itChrunk != memChurnkList.end(); ++itChrunk)
		{
			if ((*itChrunk)->spareBytes >= requestLen)
			{
				memChrunk = *itChrunk;
				return;
			}

			if ((*itChrunk)->chrunkSize >= requestLen
				&& (*itLastTimeStampChrunk)->timeStamp > (*itChrunk)->timeStamp)
			{
				itLastTimeStampChrunk = itChrunk;
			}
		}

		memChrunk = *itLastTimeStampChrunk;
		for(QHash<glyph_t, GlyphData>::Iterator itGlyph = glyphDatas.begin();
			itGlyph != glyphDatas.end(); )
		{
			if (itGlyph.value().data == memChrunk)
			{
				itGlyph = glyphDatas.erase(itGlyph);
			}
			else
			{
				++itGlyph;
			}
		}

		memChurnkList.erase(itLastTimeStampChrunk);
		if (memChrunk->chrunkSize < MAX_MEM_CHRUNK_SIZE)
		{
			MEM_TRUNK_FREE(memChrunk);
			memChrunk = (MemChrunk*)MEM_TRUNK_MALLOC(sizeof(MemChrunk) + (MAX_MEM_CHRUNK_SIZE) - 1);
			memChrunk->timeStamp = QTime::currentTime();
		}

		memChrunk->spareBytes = MAX_MEM_CHRUNK_SIZE;
		memChrunk->chrunkSize = MAX_MEM_CHRUNK_SIZE;
		::memset(memChrunk->data, 0, MAX_MEM_CHRUNK_SIZE);
		memChurnkList.push_back(memChrunk);
	}
}

void QMemTextureGlyphCache::fillTextureData(GlyphData &glyphData, glyph_t glyph)
{
	QImage mask = textureMapForGlyph(glyph);
	Q_ASSERT(m_type == QFontEngineGlyphCache::Raster_RGBMask);

	uchar* destData = glyphData.data->data + glyphData.data_offset;
	const uchar* srcData = mask.bits();
	::memcpy_s(destData, glyphData.data_len, srcData, mask.byteCount());
	glyphData.data->spareBytes -= glyphData.data_len;
	glyphData.bpl = mask.bytesPerLine();
	glyphData.isCompressed = false;
}

void QMemTextureGlyphCache::populate(QFontEngine *fontEngine, int, const glyph_t*, const QFixedPoint*)
{
	m_current_fontengine = fontEngine;
}

bool QMemTextureGlyphCache::getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, int* bpl, int* depth, int* rx, int* ry, int* w, int* h)
{
	static const qreal aliasedCoordinateDelta = 0.5 - 0.015625;
	static const QFixed offs = QFixed::fromReal(aliasedCoordinateDelta);

	if (!populateSingleGlyph(g)) {
		return false;
	}
	QHash<glyph_t, GlyphData>::iterator itGlyph = glyphDatas.find(g);
	if (itGlyph == glyphDatas.end()) {
		return false;
	}
	const GlyphData& glyphData = itGlyph.value();
	glyphData.data->timeStamp = QTime::currentTime();

	*bpl = glyphData.bpl;
	*depth = m_depth;
	*rx = qFloor(position->x + offs) + glyphData.baseLineX - glyphMargin();
	*ry = qFloor(position->y + offs) - glyphData.baseLineY - glyphMargin();
	*w = glyphData.w;
	*h = glyphData.h;
	*data = glyphData.data->data + glyphData.data_offset;
	return true;
}

/************************************************************************
* QCompressMemTextureGlyphCache
*/

//图元数据压缩以及解压缩算法

#pragma pack(push, 1)

enum FIX_GLYPH_PIX_VAL//Raster_RGBMask类型图元数据中最多出现的两类值
{
	FIX_GLYPH_PIX_EMPTY = 0xFF000000,
	FIX_GLYPH_PIX_FILLED = 0xFFFFFFFF,
};

enum FIX_GLYPH_PIX_VAL_MAP
{
	FIX_GLYPH_PIX_EMPTY_MAP = 0,
	FIX_GLYPH_PIX_FILLED_MAP = 1,
};

struct GlyphDataHeader //保证为单字节大小
{
	union
	{
		struct 
		{
			uchar isFixed : 1;
			uchar fixVal : 1;
			uchar fixValCount : 6;
		} fixGlyphDataHeader;

		struct 
		{
			uchar isFixed : 1;
			uchar unFixValCount : 7;
		} unFixGlyphDataHeader;

		uchar value;
	};
};

#pragma pack(pop)

#define MAX_FIX_VALUE_COUNT ( 1 << 6 )
#define MAX_UNFIX_VALUE_COUNT ( 1 << 7 )

//图元数据压缩算法
static bool CompressGlyphData(const quint32* src, int srcSize, uchar* dest, int* destLen)
{
	uchar* curDest = dest;
	const uchar* const destEnd = dest + *destLen;
	GlyphDataHeader* curHeader = NULL;

	struct __HeaderOp
	{
		__HeaderOp(GlyphDataHeader*& _curHeader, uchar*& _curDest, const uchar* const& _destEnd)
			: __curHeader(_curHeader), __curDest(_curDest), __destEnd(_destEnd)
		{
		}

		bool allocNew(bool isFix, uchar val = -1)
		{
			if (__curDest >= __destEnd)
				return false;

			__curHeader = (GlyphDataHeader*)__curDest;
			__curHeader->value = 0;

			if (++__curDest >= __destEnd)
				return false;
			if (isFix)
			{
				__curHeader->fixGlyphDataHeader.isFixed = 1;
				__curHeader->fixGlyphDataHeader.fixVal = val; 
				__curHeader->fixGlyphDataHeader.fixValCount = 1;
			}
			else
			{
				__curHeader->unFixGlyphDataHeader.isFixed = 0;
				__curHeader->unFixGlyphDataHeader.unFixValCount = 1;
			}
			return true;
		}

		bool update(bool isFix)
		{
			if (isFix)
			{
				if (__curHeader->fixGlyphDataHeader.fixValCount < MAX_FIX_VALUE_COUNT - 1) {
					++(__curHeader->fixGlyphDataHeader.fixValCount);
					return true;
				}
				else {
					return allocNew(true, __curHeader->fixGlyphDataHeader.fixVal);
				}
			}
			else
			{
				if (__curHeader->unFixGlyphDataHeader.unFixValCount < MAX_UNFIX_VALUE_COUNT - 1) {
					++(__curHeader->unFixGlyphDataHeader.unFixValCount);
					return true;
				}
				else {
					return allocNew(false);
				}
			}
		}
	private:
		GlyphDataHeader*& __curHeader;
		uchar*& __curDest;
		const uchar* const& __destEnd;
	};

	__HeaderOp headerOp(curHeader, curDest, destEnd);
	quint32 curSrcVal = 0;
	for (int i = 0; i < srcSize; ++i)
	{
		curSrcVal = src[i];
		if (curSrcVal == FIX_GLYPH_PIX_EMPTY)
		{
			if (curHeader != NULL 
				&& curHeader->fixGlyphDataHeader.isFixed 
				&& curHeader->fixGlyphDataHeader.fixVal == FIX_GLYPH_PIX_EMPTY_MAP )
			{
				if (!headerOp.update(true)) return false;
			}
			else
			{
				if (!headerOp.allocNew(true, FIX_GLYPH_PIX_EMPTY_MAP)) return false;
			}
		}
		else if (curSrcVal == FIX_GLYPH_PIX_FILLED)
		{
			if (curHeader != NULL 
				&& curHeader->fixGlyphDataHeader.isFixed 
				&& curHeader->fixGlyphDataHeader.fixVal == FIX_GLYPH_PIX_FILLED_MAP )
			{
				if (!headerOp.update(true)) return false;
			}
			else
			{
				if (!headerOp.allocNew(true, FIX_GLYPH_PIX_FILLED_MAP)) return false;
			}
		}
		else
		{
			if (curHeader != NULL 
				&& curHeader->fixGlyphDataHeader.isFixed == 0)
			{
				if (!headerOp.update(false)) return false;
			}
			else
			{
				if (!headerOp.allocNew(false)) return false;
			}

			if (curDest + sizeof(quint32) >= destEnd)
				return false;
			*((quint32*)curDest) = curSrcVal;
			curDest += sizeof(quint32);
		}
	}

	*destLen = curDest - dest;
	return true;
}

//图元数据解压缩算法
static bool UncompressGlyphData(const uchar* src, int srcLen, uchar* dest, int* destLen)
{
	const uchar* curSrc = src;
	const uchar* const srcEnd = src + srcLen;
	uchar* curDest = dest;
	const uchar* const destEnd = dest + *destLen;
	
	for ( ;curSrc < srcEnd; )
	{
		GlyphDataHeader curHeader = *((GlyphDataHeader*)curSrc);
		++curSrc;

		if (curHeader.fixGlyphDataHeader.isFixed)
		{
			quint32 fixGlyphDataVal = curHeader.fixGlyphDataHeader.fixVal == FIX_GLYPH_PIX_EMPTY_MAP
				? FIX_GLYPH_PIX_EMPTY : FIX_GLYPH_PIX_FILLED;

			if (curDest + sizeof(quint32) * curHeader.fixGlyphDataHeader.fixValCount > destEnd)
			{
				return false;
			}

			while(curHeader.fixGlyphDataHeader.fixValCount > 0)
			{
				*((quint32*)curDest) = fixGlyphDataVal;
				curDest += sizeof(quint32);
				--curHeader.fixGlyphDataHeader.fixValCount;
			}
		}
		else
		{
			quint32 len =  sizeof(quint32) * curHeader.unFixGlyphDataHeader.unFixValCount;
			if ( (curDest +  len >= destEnd) || (curSrc + len >= srcEnd) )
			{
				return false;
			}

			::memcpy(curDest, curSrc, len);
			curDest += len;
			curSrc += len;
		}
	}

	*destLen = curDest - dest;
	return true;
}

void QCompressMemTextureGlyphCache::fillTextureData(GlyphData &glyphData, glyph_t glyph)
{
	QImage mask = textureMapForGlyph(glyph);
	Q_ASSERT(m_type == QFontEngineGlyphCache::Raster_RGBMask);

	uchar* destData = glyphData.data->data + glyphData.data_offset;
	int destDataLen = glyphData.data_len;

	const uchar* srcData = mask.bits();
	int srcDataLen = mask.byteCount();

	if (CompressGlyphData((const quint32*)srcData, srcDataLen / sizeof(quint32), destData, &destDataLen))
	{
		glyphData.data_len = (ushort)destDataLen;
		glyphData.isCompressed = true;

		double compressRate = double(destDataLen) / srcDataLen;
		double compressRateTotal = m_avgComperssRate * m_CompressCount + compressRate;
		m_avgComperssRate = compressRateTotal / (++m_CompressCount);
	}
	else
	{
		::memcpy_s(destData, glyphData.data_len, srcData, srcDataLen);
		glyphData.isCompressed = false;
	}

	glyphData.data->spareBytes -= glyphData.data_len;
	glyphData.bpl = mask.bytesPerLine();
}

bool QCompressMemTextureGlyphCache::getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, int* bpl, int* depth, int* rx, int* ry, int* w, int* h)
{
	static const qreal aliasedCoordinateDelta = 0.5 - 0.015625;
	static const QFixed offs = QFixed::fromReal(aliasedCoordinateDelta);

	if (!populateSingleGlyph(g)) {
		return false;
	}
	QHash<glyph_t, GlyphData>::iterator itGlyph = glyphDatas.find(g);
	if (itGlyph == glyphDatas.end()) {
		return false;
	}
	const GlyphData& glyphData = itGlyph.value();
	glyphData.data->timeStamp = QTime::currentTime();

	*bpl = glyphData.bpl;
	*depth = m_depth;
	*rx = qFloor(position->x + offs) + glyphData.baseLineX - glyphMargin();
	*ry = qFloor(position->y + offs) - glyphData.baseLineY - glyphMargin();
	*w = glyphData.w;
	*h = glyphData.h;

	if (glyphData.isCompressed)
	{
		int maxGlyphDataLen = glyphData.w * glyphData.h * (m_depth >> 3);
		MemChrunk* memChurnk = NULL;
		getMemChrunk(maxGlyphDataLen, memChurnk);
		if (memChurnk == NULL)
		{
			return false;
		}
		uchar* buffer = memChurnk->data + memChurnk->chrunkSize - memChurnk->spareBytes;
		if (!UncompressGlyphData(glyphData.data->data + glyphData.data_offset,
			glyphData.data_len, buffer, &maxGlyphDataLen))
		{
			return false;
		}
		*data = buffer;
	}
	else
	{
		*data = glyphData.data->data + glyphData.data_offset;
	}
	return true;
}

#endif //[YY]

QT_END_NAMESPACE
