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

#include "qpaintdevice.h"

QT_BEGIN_NAMESPACE

extern void qt_painter_removePaintDevice(QPaintDevice *); //qpainter.cpp

QPaintDevice::QPaintDevice()
{
    painters = 0;
}

QPaintDevice::~QPaintDevice()
{
    if (paintingActive())
        qWarning("QPaintDevice: Cannot destroy paint device that is being "
                  "painted");
    qt_painter_removePaintDevice(this);
}


int QPaintDevice::metric(PaintDeviceMetric m) const
{
	//qWarning("QPaintDevice::metrics: Device has no metric information");
	//return 0;

	// Fallback: A subclass has not implemented PdmDevicePixelRatioScaled but might
	// have implemented PdmDevicePixelRatio.
	if (m == PdmDevicePixelRatioScaled)
		return this->metric(PdmDevicePixelRatio) * devicePixelRatioFScale();

	qWarning("QPaintDevice::metrics: Device has no metric information");

	if (m == PdmDpiX) {
		return 72;
	}
	else if (m == PdmDpiY) {
		return 72;
	}
	else if (m == PdmNumColors) {
		// FIXME: does this need to be a real value?
		return 256;
	}
	else if (m == PdmDevicePixelRatio) {
		return 1;
	}
	else {
		qDebug("Unrecognised metric %d!", m);
		return 0;
	}
}

Q_GUI_EXPORT int qt_paint_device_metric(const QPaintDevice *device, QPaintDevice::PaintDeviceMetric metric)
{
    return device->metric(metric);
}

QT_END_NAMESPACE
