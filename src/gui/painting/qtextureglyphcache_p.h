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

#ifndef QTEXTUREGLYPHCACHE_P_H
#define QTEXTUREGLYPHCACHE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qhash.h>
#include <qimage.h>
#include <qobject.h>
#include <qtransform.h>

#include <private/qfontengineglyphcache_p.h>

#if defined(Q_OS_VXWORKS) && defined(m_type)
#  undef m_type
#endif

#ifndef QT_DEFAULT_TEXTURE_GLYPH_CACHE_WIDTH
#define QT_DEFAULT_TEXTURE_GLYPH_CACHE_WIDTH 256
#endif

struct glyph_metrics_t;
typedef unsigned int glyph_t;


QT_BEGIN_NAMESPACE

#if 0 //[YY. zyf del]

class QTextItemInt;

class Q_GUI_EXPORT QTextureGlyphCache : public QFontEngineGlyphCache
{
public:
	QTextureGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
		: QFontEngineGlyphCache(matrix, type), m_current_fontengine(0),
		m_w(0), m_h(0), m_cx(0), m_cy(0), m_currentRowHeight(0)
	{ }

	virtual ~QTextureGlyphCache() { }

	struct Coord {
		int x;
		int y;
		int w;
		int h;

		int baseLineX;
		int baseLineY;
	};

    bool populate(QFontEngine *fontEngine, int numGlyphs, const glyph_t *glyphs,
		const QFixedPoint *positions);

	virtual void createTextureData(int width, int height) = 0;
	virtual void resizeTextureData(int width, int height) = 0;
	virtual int glyphMargin() const { return 0; }
	virtual int glyphPadding() const { return 0; }

	virtual void fillTexture(const Coord &coord, glyph_t glyph) = 0;

	inline void createCache(int width, int height) {
		m_w = width;
		m_h = height;
		createTextureData(width, height);
	}

	inline bool isNull() const { return m_w <= 0 || m_h <= 0; }

	QHash<glyph_t, Coord> coords;

	QImage textureMapForGlyph(glyph_t g) const;
	virtual int maxTextureWidth() const { return QT_DEFAULT_TEXTURE_GLYPH_CACHE_WIDTH; }
    virtual int maxTextureHeight() const { return -1; }

protected:
	QFontEngine *m_current_fontengine;

	int m_w; // image width
	int m_h; // image height
	int m_cx; // current x
	int m_cy; // current y
	int m_currentRowHeight; // Height of last row
};


class Q_GUI_EXPORT QImageTextureGlyphCache : public QTextureGlyphCache
{
public:
	QImageTextureGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
		: QTextureGlyphCache(type, matrix) { }
	virtual int glyphMargin() const;
	virtual void createTextureData(int width, int height);
	virtual void resizeTextureData(int width, int height);
	virtual void fillTexture(const Coord &c, glyph_t glyph);

	inline const QImage &image() const { return m_image; }

private:
	QImage m_image;
};

#else //[YY. zyf add]

#include <qlinkedlist.h>

class Q_GUI_EXPORT QTextureGlyphCache : public QFontEngineGlyphCache
{
public:
	static QTextureGlyphCache* createGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix);

public:
	QTextureGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
		: QFontEngineGlyphCache(matrix, type), m_current_fontengine(0)
	{
	}
	virtual ~QTextureGlyphCache() {}

public:
	virtual void populate(QFontEngine *fontEngine, int numGlyphs, const glyph_t *glyphs,
		const QFixedPoint *positions);
	virtual bool getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, 
		int* bpl, int* depth, int* rx, int* ry, int* w, int* h) = 0;
	virtual bool reachCacheBand() const { return false; }

protected:
	virtual bool findTexture(glyph_t g) = 0;
	virtual void fillTexture(glyph_t g, int w, int h, int baseLineX, int baseLineY) = 0;
	virtual int glyphMargin() const { return 0; }
	virtual int glyphPadding() const { return 0; }
	QImage textureMapForGlyph(glyph_t g) const;
	bool populateSingleGlyph(glyph_t g);

protected:
	QFontEngine *m_current_fontengine;
};

//ÒÔQTÔ­ÓÐµÄQImageÎª´æ´¢½á¹¹µÄÍ¼Ôª»º´æ
class Q_GUI_EXPORT QImageTextureGlyphCache : public QTextureGlyphCache
{
public:
	QImageTextureGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
		: QTextureGlyphCache(type, matrix), m_cx(0), m_cy(0), m_h(0), m_w(0), m_currentRowHeight(0) { }

public:
	virtual bool getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, int* bpl, int* depth, int* rx, int* ry, int* w, int* h);
	virtual bool reachCacheBand() const { return m_image.height() >= 2048; }

protected:
	virtual bool findTexture(glyph_t g);
	virtual void fillTexture(glyph_t g, int w, int h, int baseLineX, int baseLineY);
	virtual int glyphMargin() const;

private:
	struct Coord {
		int x;
		int y;
		int w;
		int h;

		int baseLineX;
		int baseLineY;
	};
	void fillTextureData(const Coord &coord, glyph_t glyph);
	void createTextureData(int width, int height);
	void resizeTextureData(int width, int height);
	inline bool isNull() const { return m_w <= 0 || m_h <= 0; }
	inline int maxTextureWidth() const { return QT_DEFAULT_TEXTURE_GLYPH_CACHE_WIDTH; }
	inline int maxTextureHeight() const { return 32768; }

private:
	QHash<glyph_t, Coord> coords;
	QImage m_image;
	int m_h;
	int m_w;
	int m_cx; // current x
	int m_cy; // current y
	int m_currentRowHeight; // Height of last row
};

//ÒÔÄÚ´æChrunkÎª´æ´¢½á¹¹µÄÍ¼Ôª»º´æ£¬½öÊÊÓÃÓÚRaster_RGBMaskÍ¼ÔªÀàÐÍ£¬ÕâÊÇ¾ø´ó¶àÊýÊÊÓÃµÄÀàÐÍ
class Q_GUI_EXPORT QMemTextureGlyphCache : public QTextureGlyphCache
{
public:
	QMemTextureGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
		: QTextureGlyphCache(type, matrix)
	{
		Q_ASSERT(type == QFontEngineGlyphCache::Raster_RGBMask);
		m_depth = 32;
	}
	virtual ~QMemTextureGlyphCache();

public:
	virtual void populate(QFontEngine *fontEngine, int numGlyphs, const glyph_t *glyphs,
		const QFixedPoint *positions);
	virtual bool getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, 
		int* bpl, int* depth, int* rx, int* ry, int* w, int* h);

public:
	virtual bool findTexture(glyph_t g);
	virtual void fillTexture(glyph_t g, int w, int h, int baseLineX, int baseLineY);
	virtual int glyphMargin() const { return 2; };

protected:
#pragma pack(push, 1)
	struct MemChrunk
	{
		QTime timeStamp;
		int spareBytes;
		int chrunkSize;
		uchar data[1];
	};
	struct GlyphData
	{
		ushort w;
		ushort h;
		short baseLineX;
		short baseLineY;
		ushort bpl : 15;
		ushort isCompressed : 1;
		MemChrunk* data;
		ushort data_offset;
		ushort data_len;
	};
#pragma pack(pop)

	virtual void getMemChrunk(int requestLen, MemChrunk*& memChrunk);
	virtual void fillTextureData(GlyphData &glyphData, glyph_t glyph);

protected:
	QLinkedList<MemChrunk*> memChurnkList;
	QHash<glyph_t, GlyphData> glyphDatas;
	int m_depth;
};

//ÒÔÄÚ´æChrunkÎª´æ´¢½á¹¹µÄÍ¼Ôª»º´æ£¬»º´æÒÔÑ¹Ëõ¸ñÊ½´æ´¢£¬ ½öÊÊÓÃÓÚRaster_RGBMaskÍ¼ÔªÀàÐÍ£¬ÕâÊÇ¾ø´ó¶àÊýÊÊÓÃµÄÀàÐÍ
class Q_GUI_EXPORT QCompressMemTextureGlyphCache : public QMemTextureGlyphCache
{
public:
	QCompressMemTextureGlyphCache(QFontEngineGlyphCache::Type type, const QTransform &matrix)
		: QMemTextureGlyphCache(type, matrix), 
		m_avgComperssRate(0.0), m_CompressCount(0)
	{
	}
	virtual ~QCompressMemTextureGlyphCache()
	{
	}

public:
	virtual bool getGlyphCacheData(glyph_t g, const QFixedPoint* position, const void** data, 
		int* bpl, int* depth, int* rx, int* ry, int* w, int* h);

protected:
	virtual void fillTextureData(GlyphData &glyphData, glyph_t glyph);

private:
	qreal m_avgComperssRate;
	uint m_CompressCount;
};

#endif //[YY]

QT_END_NAMESPACE

#endif
