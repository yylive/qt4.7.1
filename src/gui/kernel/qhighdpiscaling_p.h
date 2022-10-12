/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QHIGHDPISCALING_P_H
#define QHIGHDPISCALING_P_H
//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

//#include <QtGui/private/qtguiglobal_p.h>
#include <QtCore/qmargins.h>
#include <QtCore/qmath.h>
#include <QtCore/qrect.h>
#include <QtCore/qvector.h>
//#include <QtCore/qloggingcategory.h>
#include <QtGui/qregion.h>
//#include <QtGui/qwindow.h>

#include "qapplication_p.h"
#include "kernel/platform/qplatformintegration.h"
#include "kernel/platform/qwindowsscreen.h"

QT_BEGIN_NAMESPACE

//Q_DECLARE_LOGGING_CATEGORY(lcScaling);

class QScreen;
class QWindowsScreen;
typedef QPair<qreal, qreal> QDpi;

#ifndef QT_NO_HIGHDPISCALING
class Q_GUI_EXPORT QHighDpiScaling {
public:
    static void initHighDpiScaling();
    static void updateHighDpiScaling();

    static bool isActive() { return m_active; }
	static qreal factor(const QWidget *window);
	static qreal factor(const QWindowsScreen *platformScreen);
	static QPoint origin(const QWindowsScreen *platformScreen);
	static QPoint mapPositionFromNative(const QPoint &pos, const QWindowsScreen *platformScreen);
	static QPoint mapPositionToNative(const QPoint &pos, const QWindowsScreen *platformScreen);
    static QDpi logicalDpi();

private:
   static qreal screenSubfactor(const QWindowsScreen *screen);

    static qreal m_factor;
	static bool m_globalScalingActive;
	static bool m_usePixelDensity;
	static bool m_pixelDensityScalingActive;

    static bool m_active;
    static bool m_screenFactorSet;
    static QDpi m_logicalDpi;
};

// Coordinate system conversion functions:
// QHighDpi::fromNativePixels   : from physical(screen/backing) to logical pixels
// QHighDpi::toNativePixels              : from logical to physical pixels

namespace QHighDpi {

inline QPointF fromNative(const QPointF &pos, qreal scaleFactor, const QPointF &origin)
{
     return (pos - origin) / scaleFactor + origin;
}

inline QPointF toNative(const QPointF &pos, qreal scaleFactor, const QPointF &origin)
{
     return (pos - origin) * scaleFactor + origin;
}

inline QPoint fromNative(const QPoint &pos, qreal scaleFactor, const QPoint &origin)
{
     return (pos - origin) / scaleFactor + origin;
}

inline QPoint toNative(const QPoint &pos, qreal scaleFactor, const QPoint &origin)
{
     return (pos - origin) * scaleFactor + origin;
}

inline QPoint fromNative(const QPoint &pos, qreal scaleFactor)
{
     return pos / scaleFactor;
}

inline QPoint toNative(const QPoint &pos, qreal scaleFactor)
{
    return pos * scaleFactor;
}

inline QSize fromNative(const QSize &size, qreal scaleFactor)
{
    return size / scaleFactor; // TODO: should we round up?
}

inline QSize toNative(const QSize &size, qreal scaleFactor)
{
    return size * scaleFactor;
}

inline QSizeF fromNative(const QSizeF &size, qreal scaleFactor)
{
    return size / scaleFactor;
}

inline QSizeF toNative(const QSizeF &size, qreal scaleFactor)
{
    return size * scaleFactor;
}

inline QRect fromNative(const QRect &rect, qreal scaleFactor, const QPoint &origin)
{
    return QRect(fromNative(rect.topLeft(), scaleFactor, origin), fromNative(rect.size(), scaleFactor));
}

inline QRect toNative(const QRect &rect, qreal scaleFactor, const QPoint &origin)
{
    return QRect(toNative(rect.topLeft(), scaleFactor, origin), toNative(rect.size(), scaleFactor));

}

inline QRect fromNative(const QRect &rect, const QWindowsScreen *screen, const QPoint &screenOrigin)
{
	return fromNative(rect, QHighDpiScaling::factor(screen), screenOrigin);
}

inline QRect fromNativeScreenGeometry(const QRect &nativeScreenGeometry, const QWindowsScreen *screen)
{
	if (!QHighDpiScaling::isActive())
	{
		return nativeScreenGeometry;
	}

	return QRect(nativeScreenGeometry.topLeft(),
		fromNative(nativeScreenGeometry.size(), QHighDpiScaling::factor(screen)));
}

inline QPoint fromNativeLocalPosition(const QPoint &pos, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pos;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(window);
	return pos / scaleFactor;
}

inline QPoint toNativeLocalPosition(const QPoint &pos, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pos;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(window);
	return pos * scaleFactor;
}

inline QPointF fromNativeLocalPosition(const QPointF &pos, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pos;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(window);
	return pos / scaleFactor;
}

inline QPointF toNativeLocalPosition(const QPointF &pos, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pos;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(window);
	return pos * scaleFactor;
}


inline QRect fromNativePixels(const QRect &pixelRect, const QWindowsScreen *platformScreen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelRect;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(platformScreen);
	const QPoint origin = QHighDpiScaling::origin(platformScreen);
	return QRect(fromNative(pixelRect.topLeft(), scaleFactor, origin),
		fromNative(pixelRect.size(), scaleFactor));
}

inline QRectF fromNativePixels(const QRectF &pixelRect, const QWindowsScreen *platformScreen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelRect;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(platformScreen);
	const QPoint origin = QHighDpiScaling::origin(platformScreen);
	return QRectF(fromNative(pixelRect.topLeft(), scaleFactor, origin),
		fromNative(pixelRect.size(), scaleFactor));
}

inline QRectF toNativePixels(const QRectF &pointRect, const QWindowsScreen *platformScreen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointRect;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(platformScreen);
	const QPoint origin = QHighDpiScaling::origin(platformScreen);
	return QRectF(toNative(pointRect.topLeft(), scaleFactor, origin),
		toNative(pointRect.size(), scaleFactor));
}

inline QRect toNativePixels(const QRect &pointRect, const QWindowsScreen *platformScreen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointRect;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(platformScreen);
	const QPoint origin = QHighDpiScaling::origin(platformScreen);
	return QRect(toNative(pointRect.topLeft(), scaleFactor, origin),
		toNative(pointRect.size(), scaleFactor));
}

inline QRect fromNativePixels(const QRect &pixelRect, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelRect;
	}

	if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window)) {
		return fromNativePixels(pixelRect, QApplicationPrivate::platform_integration->screen(window));
	}
	else {
		const qreal scaleFactor = QHighDpiScaling::factor(window);
		return QRect(pixelRect.topLeft() / scaleFactor, fromNative(pixelRect.size(), scaleFactor));
	}
}

inline QRect toNativePixels(const QRect &pointRect, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointRect;
	}

	if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window)) {
		return toNativePixels(pointRect, QApplicationPrivate::platform_integration->screen(window));
	}
	else {
		const qreal scaleFactor = QHighDpiScaling::factor(window);
		return QRect(pointRect.topLeft() * scaleFactor, toNative(pointRect.size(), scaleFactor));
	}
}


inline QRectF fromNativePixels(const QRectF &pixelRect, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelRect;
	}

	if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window)) {
		return fromNativePixels(pixelRect, QApplicationPrivate::platform_integration->screen(window));
	}
	else {
		const qreal scaleFactor = QHighDpiScaling::factor(window);
		return QRectF(pixelRect.topLeft() / scaleFactor, pixelRect.size() / scaleFactor);
	}
}

inline QRectF toNativePixels(const QRectF &pointRect, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointRect;
	}

	if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window)) {
		return toNativePixels(pointRect, QApplicationPrivate::platform_integration->screen(window));
	}
	else {
		const qreal scaleFactor = QHighDpiScaling::factor(window);
		return QRectF(pointRect.topLeft() * scaleFactor, pointRect.size() * scaleFactor);
	}
}

inline QSize fromNativePixels(const QSize &pixelSize, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelSize;
	}

    return pixelSize / QHighDpiScaling::factor(window);
}

inline QSize toNativePixels(const QSize &pointSize, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointSize;
	}

	return pointSize * QHighDpiScaling::factor(window);
}

inline QSizeF fromNativePixels(const QSizeF &pixelSize, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelSize;
	}

	return pixelSize / QHighDpiScaling::factor(window);
}

inline QSizeF toNativePixels(const QSizeF &pointSize, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointSize;
	}

	return pointSize * QHighDpiScaling::factor(window);
}

inline QPoint fromNativePixels(const QPoint &pixelPoint, const QWindowsScreen *screen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelPoint;
	}

	return fromNative(pixelPoint, QHighDpiScaling::factor(screen), QHighDpiScaling::origin(screen));
}

inline QPoint fromNativePixels(const QPoint &pixelPoint, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelPoint;
	}

	if (window == NULL)
	{
		return fromNativePixels(pixelPoint, QApplicationPrivate::platform_integration->screenAtPtFromNativePixels(pixelPoint));
	}
	else if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window))
		return fromNativePixels(pixelPoint, QApplicationPrivate::platform_integration->screen(window));
	else
		return pixelPoint / QHighDpiScaling::factor(window);
}

inline QPoint toNativePixels(const QPoint &pointPoint, const QWindowsScreen *screen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointPoint;
	}

	return toNative(pointPoint, QHighDpiScaling::factor(screen), QHighDpiScaling::origin(screen));
}

inline QPoint toNativePixels(const QPoint &pointPoint, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointPoint;
	}

	if (window == NULL)
	{
		return toNativePixels(pointPoint, QApplicationPrivate::platform_integration->screenAtPtToNativePixels(pointPoint));
	} else if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window))
		return toNativePixels(pointPoint, QApplicationPrivate::platform_integration->screen(window));
	else
		return pointPoint * QHighDpiScaling::factor(window);
}

inline QPointF fromNativePixels(const QPointF &pixelPoint, const QWindowsScreen *screen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelPoint;
	}

	return fromNative(pixelPoint, QHighDpiScaling::factor(screen), QHighDpiScaling::origin(screen));
}

inline QPointF fromNativePixels(const QPointF &pixelPoint, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelPoint;
	}

	if (window == NULL)
	{
		return fromNativePixels(pixelPoint, QApplicationPrivate::platform_integration->screenAtPtFromNativePixels(pixelPoint.toPoint()));
	} else if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window))
		return fromNativePixels(pixelPoint, QApplicationPrivate::platform_integration->screen(window));
	else
		return pixelPoint / QHighDpiScaling::factor(window);
}

inline QPointF toNativePixels(const QPointF &pointPoint, const QWindowsScreen *screen)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointPoint;
	}

	return toNative(pointPoint, QHighDpiScaling::factor(screen), QHighDpiScaling::origin(screen));
}

inline QPointF toNativePixels(const QPointF &pointPoint, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointPoint;
	}

	if (window == NULL)
	{
		return toNativePixels(pointPoint, QApplicationPrivate::platform_integration->screenAtPtToNativePixels(pointPoint.toPoint()));
	}
	else if (window && window->isTopLevel() && QApplicationPrivate::platform_integration->screen(window))
        return toNativePixels(pointPoint, QApplicationPrivate::platform_integration->screen(window));
    else
        return pointPoint * QHighDpiScaling::factor(window);
}

inline QMargins fromNativePixels(const QMargins &pixelMargins, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pixelMargins;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(window);
	return QMargins(pixelMargins.left() / scaleFactor, pixelMargins.top() / scaleFactor,
		pixelMargins.right() / scaleFactor, pixelMargins.bottom() / scaleFactor);
}

inline QMargins toNativePixels(const QMargins &pointMargins, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
	{
		return pointMargins;
	}

	const qreal scaleFactor = QHighDpiScaling::factor(window);
	return QMargins(pointMargins.left() * scaleFactor, pointMargins.top() * scaleFactor,
		pointMargins.right() * scaleFactor, pointMargins.bottom() * scaleFactor);
}

inline QRegion fromNativeLocalRegion(const QRegion &pixelRegion, const QWidget *window)
{
    if (!QHighDpiScaling::isActive())
        return pixelRegion;

    qreal scaleFactor = QHighDpiScaling::factor(window);
    QRegion pointRegion;
    foreach (const QRect &rect, pixelRegion.rects()) {
        pointRegion += QRect(fromNative(rect.topLeft(), scaleFactor),
                             fromNative(rect.size(), scaleFactor));
    }
    return pointRegion;
}

// When mapping expose events to Qt rects: round top/left towards the origin and
// bottom/right away from the origin, making sure that we cover the whole window.
inline QRegion fromNativeLocalExposedRegion(const QRegion &pixelRegion, const QWidget *window)
{
    if (!QHighDpiScaling::isActive())
        return pixelRegion;

    const qreal scaleFactor = QHighDpiScaling::factor(window);
    QRegion pointRegion;
    foreach (const QRectF &rect, pixelRegion.rects()) {
        const QPointF topLeftP = rect.topLeft() / scaleFactor;
        const QSizeF sizeP = rect.size() / scaleFactor;
        pointRegion += QRect(QPoint(qFloor(topLeftP.x()), qFloor(topLeftP.y())),
                             QPoint(qCeil(topLeftP.x() + sizeP.width()  - 1.0),
                                    qCeil(topLeftP.y() + sizeP.height() - 1.0)));
    }
    return pointRegion;
}

inline QRegion toNativeLocalRegion(const QRegion &pointRegion, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
		return pointRegion;

	qreal scaleFactor = QHighDpiScaling::factor(window);
	QRegion pixelRegon;
	foreach(const QRect &rect, pointRegion.rects()) {
		pixelRegon += QRect(toNative(rect.topLeft(), scaleFactor),
			toNative(rect.size(), scaleFactor));
	}
	return pixelRegon;
}

//Any T that has operator/()
template <typename T>
T fromNativePixels(const T &pixelValue, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
		return pixelValue;

	return pixelValue / QHighDpiScaling::factor(window);

}

//##### ?????
template <typename T>
T fromNativePixels(const T &pixelValue, const QWindowsScreen *screen)
{
    if (!QHighDpiScaling::isActive())
        return pixelValue;

    return pixelValue / QHighDpiScaling::factor(screen);

}

// Any T that has operator*()
template <typename T>
T toNativePixels(const T &pointValue, const QWidget *window)
{
	if (!QHighDpiScaling::isActive())
		return pointValue;

	return pointValue * QHighDpiScaling::factor(window);
}

template <typename T>
T toNativePixels(const T &pointValue, const QWindowsScreen *screen)
{
    if (!QHighDpiScaling::isActive())
        return pointValue;

    return pointValue * QHighDpiScaling::factor(screen);
}


// Any QVector<T> where T has operator/()
//template <typename T>
//QVector<T> fromNativePixels(const QVector<T> &pixelValues, const QWindow *window)
//{
//    if (!QHighDpiScaling::isActive())
//        return pixelValues;
//
//    QVector<T> pointValues;
//    pointValues.reserve(pixelValues.size());
//    const auto factor = QHighDpiScaling::factor(window);
//    for (const T &pixelValue : pixelValues)
//        pointValues.append(pixelValue / factor);
//    return pointValues;
//}

// Any QVector<T> where T has operator*()
//template <typename T>
//QVector<T> toNativePixels(const QVector<T> &pointValues, const QWindow *window)
//{
//    if (!QHighDpiScaling::isActive())
//        return pointValues;
//
//    QVector<T> pixelValues;
//    pixelValues.reserve(pointValues.size());
//    const auto factor = QHighDpiScaling::factor(window);
//    for (const T &pointValue : pointValues)
//        pixelValues.append(pointValue * factor);
//    return pixelValues;
//}

} // namespace QHighDpi
#else // QT_NO_HIGHDPISCALING
class Q_GUI_EXPORT QHighDpiScaling {
public:
    static inline void initHighDpiScaling() {}
    static inline void updateHighDpiScaling() {}
    static inline void setGlobalFactor(qreal) {}
    static inline void setScreenFactor(QScreen *, qreal) {}

    static inline bool isActive() { return false; }
    static inline qreal factor(const QWindow *) { return 1.0; }
    static inline qreal factor(const QScreen *) { return 1.0; }
    static inline qreal factor(const QPlatformScreen *) { return 1.0; }
    static inline QPoint origin(const QScreen *) { return QPoint(); }
    static inline QPoint origin(const QPlatformScreen *) { return QPoint(); }
    static inline QPoint mapPositionFromNative(const QPoint &pos, const QPlatformScreen *) { return pos; }
    static inline QPoint mapPositionToNative(const QPoint &pos, const QPlatformScreen *) { return pos; }
    static inline QDpi logicalDpi() { return QDpi(-1,-1); }
};

namespace QHighDpi {
    template <typename T> inline
    T toNative(const T &value, ...) { return value; }
    template <typename T> inline
    T fromNative(const T &value, ...) { return value; }

    template <typename T> inline
    T fromNativeLocalPosition(const T &value, ...) { return value; }
    template <typename T> inline
    T toNativeLocalPosition(const T &value, ...) { return value; }

    template <typename T> inline
    T fromNativeLocalRegion(const T &value, ...) { return value; }
    template <typename T> inline
    T fromNativeLocalExposedRegion(const T &value, ...) { return value; }
    template <typename T> inline
    T toNativeLocalRegion(const T &value, ...) { return value; }

    template <typename T> inline
    T fromNativeScreenGeometry(const T &value, ...) { return value; }

    template <typename T, typename U> inline
    T toNativePixels(const T &value, const U*) {return value;}
    template <typename T, typename U> inline
    T fromNativePixels(const T &value, const U*) {return value;}
}
#endif // QT_NO_HIGHDPISCALING
QT_END_NAMESPACE

#endif // QHIGHDPISCALING_P_H