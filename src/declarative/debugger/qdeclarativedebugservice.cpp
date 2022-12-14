/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "private/qdeclarativedebugservice_p.h"
#include "private/qdeclarativedebugservice_p_p.h"
#include "private/qdeclarativedebugserver_p.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>

QT_BEGIN_NAMESPACE

QDeclarativeDebugServicePrivate::QDeclarativeDebugServicePrivate()
: server(0)
{
}

QDeclarativeDebugService::QDeclarativeDebugService(const QString &name, QObject *parent)
: QObject(*(new QDeclarativeDebugServicePrivate), parent)
{
    Q_D(QDeclarativeDebugService);
    d->name = name;
    d->server = QDeclarativeDebugServer::instance();
    d->status = QDeclarativeDebugService::NotConnected;

    if (!d->server)
        return;

    if (d->server->serviceNames().contains(name)) {
        qWarning() << "QDeclarativeDebugService: Conflicting plugin name" << name;
        d->server = 0;
    } else {
        d->server->addService(this);
    }
}

QDeclarativeDebugService::~QDeclarativeDebugService()
{
    Q_D(const QDeclarativeDebugService);
    if (d->server) {
        d->server->removeService(this);
    }
}

QString QDeclarativeDebugService::name() const
{
    Q_D(const QDeclarativeDebugService);
    return d->name;
}

QDeclarativeDebugService::Status QDeclarativeDebugService::status() const
{
    Q_D(const QDeclarativeDebugService);
    return d->status;
}

namespace {

    struct ObjectReference 
    {
        QPointer<QObject> object;
        int id;
    };

    struct ObjectReferenceHash 
    {
        ObjectReferenceHash() : nextId(0) {}

        QHash<QObject *, ObjectReference> objects;
        QHash<int, QObject *> ids;

        int nextId;
    };

}
Q_GLOBAL_STATIC(ObjectReferenceHash, objectReferenceHash);


/*!
    Returns a unique id for \a object.  Calling this method multiple times
    for the same object will return the same id.
*/
int QDeclarativeDebugService::idForObject(QObject *object)
{
    if (!object)
        return -1;

    ObjectReferenceHash *hash = objectReferenceHash();
    QHash<QObject *, ObjectReference>::Iterator iter = 
        hash->objects.find(object);

    if (iter == hash->objects.end()) {
        int id = hash->nextId++;

        hash->ids.insert(id, object);
        iter = hash->objects.insert(object, ObjectReference());
        iter->object = object;
        iter->id = id;
    } else if (iter->object != object) {
        int id = hash->nextId++;

        hash->ids.remove(iter->id);

        hash->ids.insert(id, object);
        iter->object = object;
        iter->id = id;
    } 
    return iter->id;
}

/*!
    Returns the object for unique \a id.  If the object has not previously been
    assigned an id, through idForObject(), then 0 is returned.  If the object
    has been destroyed, 0 is returned.
*/
QObject *QDeclarativeDebugService::objectForId(int id)
{
    ObjectReferenceHash *hash = objectReferenceHash();

    QHash<int, QObject *>::Iterator iter = hash->ids.find(id);
    if (iter == hash->ids.end())
        return 0;


    QHash<QObject *, ObjectReference>::Iterator objIter = 
        hash->objects.find(*iter);
    Q_ASSERT(objIter != hash->objects.end());

    if (objIter->object == 0) {
        hash->ids.erase(iter);
        hash->objects.erase(objIter);
        return 0;
    } else {
        return *iter;
    }
}

bool QDeclarativeDebugService::isDebuggingEnabled()
{
    return QDeclarativeDebugServer::instance() != 0;
}

bool QDeclarativeDebugService::hasDebuggingClient()
{
    return QDeclarativeDebugServer::instance() != 0
            && QDeclarativeDebugServer::instance()->hasDebuggingClient();
}

QString QDeclarativeDebugService::objectToString(QObject *obj)
{
    if(!obj)
        return QLatin1String("NULL");

    QString objectName = obj->objectName();
    if(objectName.isEmpty())
        objectName = QLatin1String("<unnamed>");

    QString rv = QString::fromUtf8(obj->metaObject()->className()) + 
                 QLatin1String(": ") + objectName;

    return rv;
}

void QDeclarativeDebugService::sendMessage(const QByteArray &message)
{
    Q_D(QDeclarativeDebugService);

    if (status() != Enabled)
        return;

    d->server->sendMessage(this, message);
}

void QDeclarativeDebugService::statusChanged(Status)
{
}

void QDeclarativeDebugService::messageReceived(const QByteArray &)
{
}

QT_END_NAMESPACE
