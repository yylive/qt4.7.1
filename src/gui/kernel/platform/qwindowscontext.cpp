/****************************************************************************
**
** Copyright (C) 2013 Samuel Gaist <samuel.gaist@edeltech.ch>
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
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
#include <private/qsystemlibrary_p.h>

#include "QDebug"
#include "qwindowsscreen.h"
#include "qwindowscontext_p.h"
#include <stdlib.h>
#include <stdio.h>
#include <windowsx.h>
#ifndef Q_OS_WINCE
#  include <comdef.h>
#endif

QT_BEGIN_NAMESPACE


#if !defined(LANG_SYRIAC)
#    define LANG_SYRIAC 0x5a
#endif

/*!
\class QWindowsUser32DLL
\brief Struct that contains dynamically resolved symbols of User32.dll.

The stub libraries shipped with the MinGW compiler miss some of the
functions. They need to be retrieved dynamically.

In addition, touch-related functions are available only from Windows onwards.
These need to resolved dynamically for Q_CC_MSVC as well.

\sa QWindowsShell32DLL

\internal
\ingroup qt-lighthouse-win
*/
#ifndef Q_OS_WINCE
QWindowsUser32DLL::QWindowsUser32DLL() :
	setLayeredWindowAttributes(0), updateLayeredWindow(0),
	updateLayeredWindowIndirect(0),
	isHungAppWindow(0), isTouchWindow(0),
	registerTouchWindow(0), unregisterTouchWindow(0),
	getTouchInputInfo(0), closeTouchInputHandle(0), setProcessDPIAware(0),
	addClipboardFormatListener(0), removeClipboardFormatListener(0),
	getDisplayAutoRotationPreferences(0), setDisplayAutoRotationPreferences(0)
{
}

void QWindowsUser32DLL::init()
{
	QSystemLibrary library(L"user32");
	// MinGW (g++ 3.4.5) accepts only C casts.
	setLayeredWindowAttributes = (SetLayeredWindowAttributes)(library.resolve("SetLayeredWindowAttributes"));
	updateLayeredWindow = (UpdateLayeredWindow)(library.resolve("UpdateLayeredWindow"));
	if (Q_UNLIKELY(!setLayeredWindowAttributes || !updateLayeredWindow))
		qFatal("This version of Windows is not supported (User32.dll is missing the symbols 'SetLayeredWindowAttributes', 'UpdateLayeredWindow').");

	updateLayeredWindowIndirect = (UpdateLayeredWindowIndirect)(library.resolve("UpdateLayeredWindowIndirect"));
	isHungAppWindow = (IsHungAppWindow)library.resolve("IsHungAppWindow");
	setProcessDPIAware = (SetProcessDPIAware)library.resolve("SetProcessDPIAware");

	if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA) {
		addClipboardFormatListener = (AddClipboardFormatListener)library.resolve("AddClipboardFormatListener");
		removeClipboardFormatListener = (RemoveClipboardFormatListener)library.resolve("RemoveClipboardFormatListener");
	}
	getDisplayAutoRotationPreferences = (GetDisplayAutoRotationPreferences)library.resolve("GetDisplayAutoRotationPreferences");
	setDisplayAutoRotationPreferences = (SetDisplayAutoRotationPreferences)library.resolve("SetDisplayAutoRotationPreferences");
}

bool QWindowsUser32DLL::initTouch()
{
	if (!isTouchWindow && QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
		QSystemLibrary library(L"user32");
		isTouchWindow = (IsTouchWindow)(library.resolve("IsTouchWindow"));
		registerTouchWindow = (RegisterTouchWindow)(library.resolve("RegisterTouchWindow"));
		unregisterTouchWindow = (UnregisterTouchWindow)(library.resolve("UnregisterTouchWindow"));
		getTouchInputInfo = (GetTouchInputInfo)(library.resolve("GetTouchInputInfo"));
		closeTouchInputHandle = (CloseTouchInputHandle)(library.resolve("CloseTouchInputHandle"));
	}
	return isTouchWindow && registerTouchWindow && unregisterTouchWindow && getTouchInputInfo && closeTouchInputHandle;
}

/*!
\class QWindowsShell32DLL
\brief Struct that contains dynamically resolved symbols of Shell32.dll.

The stub libraries shipped with the MinGW compiler miss some of the
functions. They need to be retrieved dynamically.

\sa QWindowsUser32DLL

\internal
\ingroup qt-lighthouse-win
*/

QWindowsShell32DLL::QWindowsShell32DLL()
	: sHCreateItemFromParsingName(0)
	, sHGetKnownFolderIDList(0)
	, sHGetStockIconInfo(0)
	, sHGetImageList(0)
	, sHCreateItemFromIDList(0)
{
}

void QWindowsShell32DLL::init()
{
	QSystemLibrary library(L"shell32");
	sHCreateItemFromParsingName = (SHCreateItemFromParsingName)(library.resolve("SHCreateItemFromParsingName"));
	sHGetKnownFolderIDList = (SHGetKnownFolderIDList)(library.resolve("SHGetKnownFolderIDList"));
	sHGetStockIconInfo = (SHGetStockIconInfo)library.resolve("SHGetStockIconInfo");
	sHGetImageList = (SHGetImageList)library.resolve("SHGetImageList");
	sHCreateItemFromIDList = (SHCreateItemFromIDList)library.resolve("SHCreateItemFromIDList");
}


QWindowsShcoreDLL::QWindowsShcoreDLL()
	: getProcessDpiAwareness(0)
	, setProcessDpiAwareness(0)
	, getDpiForMonitor(0)
{
}

void QWindowsShcoreDLL::init()
{
	if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
		return;
	QSystemLibrary library(L"SHCore");
	getProcessDpiAwareness = (GetProcessDpiAwareness)library.resolve("GetProcessDpiAwareness");
	setProcessDpiAwareness = (SetProcessDpiAwareness)library.resolve("SetProcessDpiAwareness");
	getDpiForMonitor = (GetDpiForMonitor)library.resolve("GetDpiForMonitor");
}

QWindowsUser32DLL QWindowsContext::user32dll;
QWindowsShell32DLL QWindowsContext::shell32dll;
QWindowsShcoreDLL QWindowsContext::shcoredll;

#endif // !Q_OS_WINCE

QWindowsContext *QWindowsContext::m_instance = 0;

/*!
\class QWindowsContext
\brief Singleton container for all relevant information.

Holds state information formerly stored in \c qapplication_win.cpp.

\internal
\ingroup qt-lighthouse-win
*/

struct QWindowsContextPrivate {

	QWindowsContextPrivate();
	
	HDC m_displayContext;
	int m_defaultDPI;
	
	QWindowsScreenManager m_screenManager;
};

QWindowsContextPrivate::QWindowsContextPrivate()
{
#ifndef Q_OS_WINCE
	QWindowsContext::shcoredll.init();
	QWindowsContext::user32dll.init();
	QWindowsContext::shell32dll.init();
#endif // !Q_OS_WINCE
}

QWindowsContext::QWindowsContext() : d(new QWindowsContextPrivate)
{
	m_instance = this;
}


QWindowsContext::~QWindowsContext()
{
	d->m_screenManager.clearScreens(); // Order: Potentially calls back to the windows.
	m_instance = 0;
}

QWindowsScreenManager & QWindowsContext::screenManager()
{
	return d->m_screenManager;
}

void QWindowsContext::setProcessDpiAwareness(ProcessDpiAwareness dpiAwareness)
{
	qDebug() << Q_FUNC_INFO << dpiAwareness;

	if (QWindowsContext::shcoredll.isValid()) {
		const HRESULT hr = QWindowsContext::shcoredll.setProcessDpiAwareness(dpiAwareness);
		// E_ACCESSDENIED means set externally (MSVC manifest or external app loading Qt plugin).
		// Silence warning in that case unless debug is enabled.
		if (FAILED(hr) && hr != E_ACCESSDENIED) {
			qWarning() << "SetProcessDpiAwareness("
				<< dpiAwareness << ") failed: " << QWindowsContext::comErrorString(hr)
				<< ", using " << QWindowsContext::processDpiAwareness();
		}
	} else {
		if (dpiAwareness != ProcessDpiUnaware && QWindowsContext::user32dll.setProcessDPIAware) {
			if (!QWindowsContext::user32dll.setProcessDPIAware())
				qErrnoWarning("SetProcessDPIAware() failed");
		}
	}
}

int QWindowsContext::processDpiAwareness()
{
#ifndef Q_OS_WINCE
	int result;
	if (QWindowsContext::shcoredll.getProcessDpiAwareness
		&& SUCCEEDED(QWindowsContext::shcoredll.getProcessDpiAwareness(NULL, &result))) {
			return result;
	}
#endif // !Q_OS_WINCE
	return -1;
}

QByteArray QWindowsContext::comErrorString(HRESULT hr)
{
	QByteArray result = QByteArray("COM error 0x") + QByteArray::number(quintptr(hr), 16) + ' ';
	switch (hr) {
case S_OK:
	result += "S_OK";
	break;
case S_FALSE:
	result += "S_FALSE";
	break;
case E_UNEXPECTED:
	result += "E_UNEXPECTED";
	break;
case E_ACCESSDENIED:
	result += "E_ACCESSDENIED";
	break;
case CO_E_ALREADYINITIALIZED:
	result += "CO_E_ALREADYINITIALIZED";
	break;
case CO_E_NOTINITIALIZED:
	result += "CO_E_NOTINITIALIZED";
	break;
case RPC_E_CHANGED_MODE:
	result += "RPC_E_CHANGED_MODE";
	break;
case OLE_E_WRONGCOMPOBJ:
	result += "OLE_E_WRONGCOMPOBJ";
	break;
case CO_E_NOT_SUPPORTED:
	result += "CO_E_NOT_SUPPORTED";
	break;
case E_NOTIMPL:
	result += "E_NOTIMPL";
	break;
case E_INVALIDARG:
	result += "E_INVALIDARG";
	break;
case E_NOINTERFACE:
	result += "E_NOINTERFACE";
	break;
case E_POINTER:
	result += "E_POINTER";
	break;
case E_HANDLE:
	result += "E_HANDLE";
	break;
case E_ABORT:
	result += "E_ABORT";
	break;
case E_FAIL:
	result += "E_FAIL";
	break;
case RPC_E_WRONG_THREAD:
	result += "RPC_E_WRONG_THREAD";
	break;
case RPC_E_THREAD_NOT_INIT:
	result += "RPC_E_THREAD_NOT_INIT";
	break;
default:
	break;
	}
#ifndef Q_OS_WINCE
	_com_error error(hr);
	result += " (";
	//result += errorMessageFromComError(error);
	result += ')';
#endif // !Q_OS_WINCE
	return result;
}

QT_END_NAMESPACE


