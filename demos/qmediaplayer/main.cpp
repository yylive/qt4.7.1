/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
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
***************************************************************************/

#include <QtGui>
#include "mediaplayer.h"

const qreal DefaultVolume = -1.0;

int main (int argc, char *argv[])
{
    Q_INIT_RESOURCE(mediaplayer);
    QApplication app(argc, argv);
    app.setApplicationName("Media Player");
    app.setOrganizationName("Qt");
    app.setQuitOnLastWindowClosed(true);

    QString fileName;
    qreal volume = DefaultVolume;
    bool smallScreen = false;
#ifdef Q_OS_SYMBIAN
    smallScreen = true;
#endif

    QStringList args(app.arguments());
    args.removeFirst(); // remove name of executable
    while (!args.empty()) {
        const QString &arg = args.first();
        if (QLatin1String("-small-screen") == arg || QLatin1String("--small-screen") == arg) {
            smallScreen = true;
        } else if (QLatin1String("-volume") == arg || QLatin1String("--volume") == arg) {
            if (!args.empty()) {
                args.removeFirst();
                volume = qMax(qMin(args.first().toFloat(), float(1.0)), float(0.0));
            }
        } else if (fileName.isNull()) {
            fileName = arg;
        }
        args.removeFirst();
    }

    MediaPlayer player;
    player.setSmallScreen(smallScreen);
    if (DefaultVolume != volume)
        player.setVolume(volume);
    if (!fileName.isNull())
        player.setFile(fileName);

    if (smallScreen)
        player.showMaximized();
    else
        player.show();

    return app.exec();
}

