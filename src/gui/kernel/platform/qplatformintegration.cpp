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
#include "kernel/qhighdpiscaling_p.h"
#include "qwindowsscreen.h"
#include "qwindowscontext_p.h"
#include "qplatformintegration.h"

QT_BEGIN_NAMESPACE


struct QPlatformIntegrationPrivate
{
	QPlatformIntegrationPrivate();

	QWindowsContext m_context;
};

QPlatformIntegrationPrivate::QPlatformIntegrationPrivate()
{
}

QPlatformIntegration::QPlatformIntegration()
	: d(new QPlatformIntegrationPrivate)
{
	static bool dpiAwarenessSet = false;	
	if (!dpiAwarenessSet) { // Set only once in case of repeated instantiations of QGuiApplication.
		if (QCoreApplication::testAttribute(Qt::AA_EnableHighDpiScaling)) {
			d->m_context.setProcessDpiAwareness(QWindowsContext::ProcessPerMonitorDpiAware);
		}
		else if (QCoreApplication::testAttribute(Qt::AA_DisableHighDpiScaling)) {
			d->m_context.setProcessDpiAwareness(QWindowsContext::ProcessDpiUnaware);
		}
		dpiAwarenessSet = true;
	}

	QHighDpiScaling::initHighDpiScaling();
}

QPlatformIntegration::~QPlatformIntegration()
{

}

void QPlatformIntegration::updateDpiSetting()
{
	d->m_context.screenManager().handleScreenChanges();
	QHighDpiScaling::updateHighDpiScaling();	
}

QWindowsScreen* QPlatformIntegration::screen(const QWidget *w)
{
	Q_UNUSED(w);
	return primaryScreen();
}

QWindowsScreen * QPlatformIntegration::screen(const HWND wndId)
{
	Q_UNUSED(wndId);
	return primaryScreen();
}

QList<QWindowsScreen *> QPlatformIntegration::screens()
{
	return d->m_context.screenManager().screens();
}

QWindowsScreen* QPlatformIntegration::screenAtPtFromNativePixels(const QPoint& logicPoint)
{
	int index = QApplication::desktop()->screenNumber(logicPoint);
	if (QApplicationPrivate::platform_integration->screens().count() <= index)
	{
		return primaryScreen();
	}
	else
	{
		return screens().at(index);
	}
}

QWindowsScreen* QPlatformIntegration::screenAtPtToNativePixels(const QPoint& physicalPoint)
{
	QWindowsScreen* screen = const_cast<QWindowsScreen*>(d->m_context.screenManager().screenAtDp(physicalPoint));
	if (screen == NULL)
	{
		screen = primaryScreen();
	}

	return screen;
}

QWindowsScreen* QPlatformIntegration::primaryScreen()
{
	if (d->m_context.screenManager().screens().isEmpty())
	{
		return 0;
	}
	else
	{
		return d->m_context.screenManager().screens().first();
	}
}

QT_END_NAMESPACE



