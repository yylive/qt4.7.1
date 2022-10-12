/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
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

#ifndef BLUETOOTHLISTENER_GUI_H
#define BLUETOOTHLISTENER_GUI_H

#include "symbianutils_global.h"

#include <QtCore/QtGlobal>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace trk {
class BaseCommunicationStarter;

/* promptStartCommunication(): Convenience functions that
 * prompt the user to start a communication (launching or
 * connecting TRK) using a modal message box in which they can cancel.
 * Pass in the starter with device and parameters set up.  */

enum PromptStartCommunicationResult {
    PromptStartCommunicationConnected,
    PromptStartCommunicationCanceled,
    PromptStartCommunicationError
};

SYMBIANUTILS_EXPORT PromptStartCommunicationResult
        promptStartCommunication(BaseCommunicationStarter &starter,
                                 const QString &msgBoxTitle,
                                 const QString &msgBoxText,
                                 QWidget *msgBoxParent,
                                 QString *errorMessage);

// Convenience to start a serial connection (messages prompting
// to launch Trk).
SYMBIANUTILS_EXPORT PromptStartCommunicationResult
        promptStartSerial(BaseCommunicationStarter &starter,
                          QWidget *msgBoxParent,
                          QString *errorMessage);

// Convenience to start blue tooth connection (messages
// prompting to connect).
SYMBIANUTILS_EXPORT PromptStartCommunicationResult
        promptStartBluetooth(BaseCommunicationStarter &starter,
                             QWidget *msgBoxParent,
                             QString *errorMessage);
} // namespace trk

#endif // BLUETOOTHLISTENER_GUI_H
