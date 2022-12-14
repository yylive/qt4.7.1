/****************************************************************************
**
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

#ifndef QWINDOWSCONTEXT_H
#define QWINDOWSCONTEXT_H

//#include "qtwindowsglobal.h"
//#include "qtwindows_additional.h"

#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
//#include <QtCore/QLoggingCategory>

#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <shlwapi.h>

struct IBindCtx;
struct _SHSTOCKICONINFO;

QT_BEGIN_NAMESPACE


#ifndef Q_OS_WINCE
struct QWindowsUser32DLL
{
	QWindowsUser32DLL();
	inline void init();
	inline bool initTouch();

	typedef BOOL(WINAPI *IsTouchWindow)(HWND, PULONG);
	typedef BOOL(WINAPI *RegisterTouchWindow)(HWND, ULONG);
	typedef BOOL(WINAPI *UnregisterTouchWindow)(HWND);
	typedef BOOL(WINAPI *GetTouchInputInfo)(HANDLE, UINT, PVOID, int);
	typedef BOOL(WINAPI *CloseTouchInputHandle)(HANDLE);
	typedef BOOL(WINAPI *SetLayeredWindowAttributes)(HWND, COLORREF, BYTE, DWORD);
	typedef BOOL(WINAPI *UpdateLayeredWindow)(HWND, HDC, const POINT *,
		const SIZE *, HDC, const POINT *, COLORREF,
		const BLENDFUNCTION *, DWORD);
	typedef BOOL(WINAPI *UpdateLayeredWindowIndirect)(HWND, const UPDATELAYEREDWINDOWINFO *);
	typedef BOOL(WINAPI *IsHungAppWindow)(HWND);
	typedef BOOL(WINAPI *SetProcessDPIAware)();
	typedef BOOL(WINAPI *AddClipboardFormatListener)(HWND);
	typedef BOOL(WINAPI *RemoveClipboardFormatListener)(HWND);
	typedef BOOL(WINAPI *GetDisplayAutoRotationPreferences)(DWORD *);
	typedef BOOL(WINAPI *SetDisplayAutoRotationPreferences)(DWORD);

	// Functions missing in Q_CC_GNU stub libraries.
	SetLayeredWindowAttributes setLayeredWindowAttributes;
	UpdateLayeredWindow updateLayeredWindow;

	// Functions missing in older versions of Windows
	UpdateLayeredWindowIndirect updateLayeredWindowIndirect;
	IsHungAppWindow isHungAppWindow;

	// Touch functions from Windows 7 onwards (also for use with Q_CC_MSVC).
	IsTouchWindow isTouchWindow;
	RegisterTouchWindow registerTouchWindow;
	UnregisterTouchWindow unregisterTouchWindow;
	GetTouchInputInfo getTouchInputInfo;
	CloseTouchInputHandle closeTouchInputHandle;

	// Windows Vista onwards
	SetProcessDPIAware setProcessDPIAware;

	// Clipboard listeners, Windows Vista onwards
	AddClipboardFormatListener addClipboardFormatListener;
	RemoveClipboardFormatListener removeClipboardFormatListener;

	// Rotation API
	GetDisplayAutoRotationPreferences getDisplayAutoRotationPreferences;
	SetDisplayAutoRotationPreferences setDisplayAutoRotationPreferences;
};

struct QWindowsShell32DLL
{
	QWindowsShell32DLL();
	inline void init();

	typedef HRESULT(WINAPI *SHCreateItemFromParsingName)(PCWSTR, IBindCtx *, const GUID&, void **);
	typedef HRESULT(WINAPI *SHGetKnownFolderIDList)(const GUID &, DWORD, HANDLE, PIDLIST_ABSOLUTE *);
	typedef HRESULT(WINAPI *SHGetStockIconInfo)(int, int, _SHSTOCKICONINFO *);
	typedef HRESULT(WINAPI *SHGetImageList)(int, REFIID, void **);
	typedef HRESULT(WINAPI *SHCreateItemFromIDList)(PCIDLIST_ABSOLUTE, REFIID, void **);

	SHCreateItemFromParsingName sHCreateItemFromParsingName;
	SHGetKnownFolderIDList sHGetKnownFolderIDList;
	SHGetStockIconInfo sHGetStockIconInfo;
	SHGetImageList sHGetImageList;
	SHCreateItemFromIDList sHCreateItemFromIDList;
};

// Shell scaling library (Windows 8.1 onwards)
struct QWindowsShcoreDLL {
	QWindowsShcoreDLL();
	void init();
	inline bool isValid() const { return getProcessDpiAwareness && setProcessDpiAwareness && getDpiForMonitor; }

	typedef HRESULT(WINAPI *GetProcessDpiAwareness)(HANDLE, int *);
	typedef HRESULT(WINAPI *SetProcessDpiAwareness)(int);
	typedef HRESULT(WINAPI *GetDpiForMonitor)(HMONITOR, int, UINT *, UINT *);

	GetProcessDpiAwareness getProcessDpiAwareness;
	SetProcessDpiAwareness setProcessDpiAwareness;
	GetDpiForMonitor getDpiForMonitor;
};

#endif // Q_OS_WINCE
class QWindowsScreenManager;
struct QWindowsContextPrivate;
class QWindowsContext
{
	Q_DISABLE_COPY(QWindowsContext)
public:

	explicit QWindowsContext();
	virtual ~QWindowsContext();

	QWindowsScreenManager &screenManager();

#ifndef Q_OS_WINCE
	static QWindowsUser32DLL user32dll;
	static QWindowsShell32DLL shell32dll;
	static QWindowsShcoreDLL shcoredll;
#endif
	QScopedPointer<QWindowsContextPrivate> d;
	static QWindowsContext *m_instance;
};

//extern "C" LRESULT QT_WIN_CALLBACK qWindowsWndProc(HWND, UINT, WPARAM, LPARAM);

QT_END_NAMESPACE

#endif // QWINDOWSCONTEXT_H
