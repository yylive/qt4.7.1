/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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

#ifndef QMEEGOOVERLAYWIDGET_H
#define QMEEGOOVERLAYWIDGET_H

#include <QWidget>

//! A widget automatically scaling it's content.
/*!
*/

class Q_DECL_EXPORT QMeeGoOverlayWidget : public QWidget
{
public:
    //! Constructs a new scaling widget.
    /*!
     The real surface used for this widget will have the specified
     width and height.
    */
    QMeeGoOverlayWidget(int surfaceWidth, int surfaceHeight, QWidget *parent = 0);


    //! Event filtering function. 
    /*!
      Converts coordinates for mouse/touch event. Do not
      call manually.
    */
    bool eventFilter(QObject *obj, QEvent *event);

    //! Standard override.
    /*!
     The surface scaling on the target paint device is being
     set when the widget is displayed for the first time.
    */
    virtual void showEvent(QShowEvent *event);

private:
    //! Converts coordinates between real & virtual area of the widget.
    QPoint convertPoint(const QPoint &p);

    int sw; /// Surface real width.
    int sh; /// Surface real height.
    float scaleW; /// Width scaling factor.
    float scaleH; /// Height scaling factor.
};

#endif
