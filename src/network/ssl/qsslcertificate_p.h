/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtNetwork module of the Qt Toolkit.
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


#ifndef QSSLCERTIFICATE_P_H
#define QSSLCERTIFICATE_P_H

#include "qsslcertificate.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qsslsocket_p.h"
#include <QtCore/qdatetime.h>
#include <QtCore/qmap.h>

#include <openssl/x509.h>

QT_BEGIN_NAMESPACE

class QSslCertificatePrivate
{
public:
    QSslCertificatePrivate()
        : null(true), x509(0)
    {
        QSslSocketPrivate::ensureInitialized();
    }

    ~QSslCertificatePrivate()
    {
        if (x509)
            q_X509_free(x509);
    }

    bool null;
    QByteArray versionString;
    QByteArray serialNumberString;

    QMap<QString, QString> issuerInfo;
    QMap<QString, QString> subjectInfo;
    QDateTime notValidAfter;
    QDateTime notValidBefore;

    X509 *x509;

    void init(const QByteArray &data, QSsl::EncodingFormat format);

    static QByteArray QByteArray_from_X509(X509 *x509, QSsl::EncodingFormat format);
    static QSslCertificate QSslCertificate_from_X509(X509 *x509);
    static QList<QSslCertificate> certificatesFromPem(const QByteArray &pem, int count = -1);
    static QList<QSslCertificate> certificatesFromDer(const QByteArray &der, int count = -1);
    static bool isBlacklisted(const QSslCertificate &certificate);

    friend class QSslSocketBackendPrivate;

    QAtomicInt ref;
};

QT_END_NAMESPACE

#endif
