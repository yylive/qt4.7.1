/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "private/qdeclarativeutilmodule_p.h"
#include "private/qdeclarativeanimation_p.h"
#include "private/qdeclarativeanimation_p_p.h"
#include "private/qdeclarativebehavior_p.h"
#include "private/qdeclarativebind_p.h"
#include "private/qdeclarativeconnections_p.h"
#include "private/qdeclarativesmoothedanimation_p.h"
#include "private/qdeclarativefontloader_p.h"
#include "private/qdeclarativelistaccessor_p.h"
#include "private/qdeclarativelistmodel_p.h"
#include "private/qdeclarativenullablevalue_p_p.h"
#include "private/qdeclarativeopenmetaobject_p.h"
#include "private/qdeclarativepackage_p.h"
#include "private/qdeclarativepixmapcache_p.h"
#include "private/qdeclarativepropertychanges_p.h"
#include "qdeclarativepropertymap.h"
#include "private/qdeclarativespringanimation_p.h"
#include "private/qdeclarativestategroup_p.h"
#include "private/qdeclarativestateoperations_p.h"
#include "private/qdeclarativestate_p.h"
#include "private/qdeclarativestate_p_p.h"
#include "private/qdeclarativestyledtext_p.h"
#include "private/qdeclarativesystempalette_p.h"
#include "private/qdeclarativetimeline_p_p.h"
#include "private/qdeclarativetimer_p.h"
#include "private/qdeclarativetransitionmanager_p_p.h"
#include "private/qdeclarativetransition_p.h"
#include "private/qdeclarativeapplication_p.h"
#include "qdeclarativeview.h"
#include "qdeclarativeinfo.h"
#include "private/qdeclarativetypenotavailable_p.h"
#ifndef QT_NO_XMLPATTERNS
#include "private/qdeclarativexmllistmodel_p.h"
#endif

void QDeclarativeUtilModule::defineModule()
{
    qmlRegisterUncreatableType<QDeclarativeApplication>("QtQuick",1,1,"Application", QDeclarativeApplication::tr("Application is an abstract class"));

    qmlRegisterType<QDeclarativeAnchorAnimation>("QtQuick",1,0,"AnchorAnimation");
    qmlRegisterType<QDeclarativeAnchorChanges>("QtQuick",1,0,"AnchorChanges");
    qmlRegisterType<QDeclarativeBehavior>("QtQuick",1,0,"Behavior");
    qmlRegisterType<QDeclarativeBind>("QtQuick",1,0,"Binding");
    qmlRegisterType<QDeclarativeColorAnimation>("QtQuick",1,0,"ColorAnimation");
    qmlRegisterType<QDeclarativeConnections>("QtQuick",1,0,"Connections");
    qmlRegisterType<QDeclarativeSmoothedAnimation>("QtQuick",1,0,"SmoothedAnimation");
    qmlRegisterType<QDeclarativeFontLoader>("QtQuick",1,0,"FontLoader");
    qmlRegisterType<QDeclarativeListElement>("QtQuick",1,0,"ListElement");
    qmlRegisterType<QDeclarativeNumberAnimation>("QtQuick",1,0,"NumberAnimation");
    qmlRegisterType<QDeclarativePackage>("QtQuick",1,0,"Package");
    qmlRegisterType<QDeclarativeParallelAnimation>("QtQuick",1,0,"ParallelAnimation");
    qmlRegisterType<QDeclarativeParentAnimation>("QtQuick",1,0,"ParentAnimation");
    qmlRegisterType<QDeclarativeParentChange>("QtQuick",1,0,"ParentChange");
    qmlRegisterType<QDeclarativePauseAnimation>("QtQuick",1,0,"PauseAnimation");
    qmlRegisterType<QDeclarativePropertyAction>("QtQuick",1,0,"PropertyAction");
    qmlRegisterType<QDeclarativePropertyAnimation>("QtQuick",1,0,"PropertyAnimation");
    qmlRegisterType<QDeclarativeRotationAnimation>("QtQuick",1,0,"RotationAnimation");
    qmlRegisterType<QDeclarativeScriptAction>("QtQuick",1,0,"ScriptAction");
    qmlRegisterType<QDeclarativeSequentialAnimation>("QtQuick",1,0,"SequentialAnimation");
    qmlRegisterType<QDeclarativeSpringAnimation>("QtQuick",1,0,"SpringAnimation");
    qmlRegisterType<QDeclarativeStateChangeScript>("QtQuick",1,0,"StateChangeScript");
    qmlRegisterType<QDeclarativeStateGroup>("QtQuick",1,0,"StateGroup");
    qmlRegisterType<QDeclarativeState>("QtQuick",1,0,"State");
    qmlRegisterType<QDeclarativeSystemPalette>("QtQuick",1,0,"SystemPalette");
    qmlRegisterType<QDeclarativeTimer>("QtQuick",1,0,"Timer");
    qmlRegisterType<QDeclarativeTransition>("QtQuick",1,0,"Transition");
    qmlRegisterType<QDeclarativeVector3dAnimation>("QtQuick",1,0,"Vector3dAnimation");
#ifdef QT_NO_XMLPATTERNS
    qmlRegisterTypeNotAvailable("QtQuick",1,0,"XmlListModel",
        qApp->translate("QDeclarativeXmlListModel","Qt was built without support for xmlpatterns"));
    qmlRegisterTypeNotAvailable("QtQuick",1,0,"XmlRole",
        qApp->translate("QDeclarativeXmlListModel","Qt was built without support for xmlpatterns"));
#else
    qmlRegisterType<QDeclarativeXmlListModel>("QtQuick",1,0,"XmlListModel");
    qmlRegisterType<QDeclarativeXmlListModelRole>("QtQuick",1,0,"XmlRole");
#endif

    qmlRegisterType<QDeclarativeAnchors>();
    qmlRegisterType<QDeclarativeStateOperation>();
    qmlRegisterType<QDeclarativeAnchorSet>();

    qmlRegisterUncreatableType<QDeclarativeAbstractAnimation>("QtQuick",1,0,"Animation",QDeclarativeAbstractAnimation::tr("Animation is an abstract class"));

    qmlRegisterCustomType<QDeclarativeListModel>("QtQuick",1,0,"ListModel", new QDeclarativeListModelParser);
    qmlRegisterCustomType<QDeclarativePropertyChanges>("QtQuick",1,0,"PropertyChanges", new QDeclarativePropertyChangesParser);
    qmlRegisterCustomType<QDeclarativeConnections>("QtQuick",1,0,"Connections", new QDeclarativeConnectionsParser);

#ifndef QT_NO_IMPORT_QT47_QML
    qmlRegisterType<QDeclarativeAnchorAnimation>("Qt",4,7,"AnchorAnimation");
    qmlRegisterType<QDeclarativeAnchorChanges>("Qt",4,7,"AnchorChanges");
    qmlRegisterType<QDeclarativeBehavior>("Qt",4,7,"Behavior");
    qmlRegisterType<QDeclarativeBind>("Qt",4,7,"Binding");
    qmlRegisterType<QDeclarativeColorAnimation>("Qt",4,7,"ColorAnimation");
    qmlRegisterType<QDeclarativeConnections>("Qt",4,7,"Connections");
    qmlRegisterType<QDeclarativeSmoothedAnimation>("Qt",4,7,"SmoothedAnimation");
    qmlRegisterType<QDeclarativeFontLoader>("Qt",4,7,"FontLoader");
    qmlRegisterType<QDeclarativeListElement>("Qt",4,7,"ListElement");
    qmlRegisterType<QDeclarativeNumberAnimation>("Qt",4,7,"NumberAnimation");
    qmlRegisterType<QDeclarativePackage>("Qt",4,7,"Package");
    qmlRegisterType<QDeclarativeParallelAnimation>("Qt",4,7,"ParallelAnimation");
    qmlRegisterType<QDeclarativeParentAnimation>("Qt",4,7,"ParentAnimation");
    qmlRegisterType<QDeclarativeParentChange>("Qt",4,7,"ParentChange");
    qmlRegisterType<QDeclarativePauseAnimation>("Qt",4,7,"PauseAnimation");
    qmlRegisterType<QDeclarativePropertyAction>("Qt",4,7,"PropertyAction");
    qmlRegisterType<QDeclarativePropertyAnimation>("Qt",4,7,"PropertyAnimation");
    qmlRegisterType<QDeclarativeRotationAnimation>("Qt",4,7,"RotationAnimation");
    qmlRegisterType<QDeclarativeScriptAction>("Qt",4,7,"ScriptAction");
    qmlRegisterType<QDeclarativeSequentialAnimation>("Qt",4,7,"SequentialAnimation");
    qmlRegisterType<QDeclarativeSpringAnimation>("Qt",4,7,"SpringAnimation");
    qmlRegisterType<QDeclarativeStateChangeScript>("Qt",4,7,"StateChangeScript");
    qmlRegisterType<QDeclarativeStateGroup>("Qt",4,7,"StateGroup");
    qmlRegisterType<QDeclarativeState>("Qt",4,7,"State");
    qmlRegisterType<QDeclarativeSystemPalette>("Qt",4,7,"SystemPalette");
    qmlRegisterType<QDeclarativeTimer>("Qt",4,7,"Timer");
    qmlRegisterType<QDeclarativeTransition>("Qt",4,7,"Transition");
    qmlRegisterType<QDeclarativeVector3dAnimation>("Qt",4,7,"Vector3dAnimation");
#ifdef QT_NO_XMLPATTERNS
    qmlRegisterTypeNotAvailable("Qt",4,7,"XmlListModel",
        qApp->translate("QDeclarativeXmlListModel","Qt was built without support for xmlpatterns"));
    qmlRegisterTypeNotAvailable("Qt",4,7,"XmlRole",
        qApp->translate("QDeclarativeXmlListModel","Qt was built without support for xmlpatterns"));
#else
    qmlRegisterType<QDeclarativeXmlListModel>("Qt",4,7,"XmlListModel");
    qmlRegisterType<QDeclarativeXmlListModelRole>("Qt",4,7,"XmlRole");
#endif

    qmlRegisterUncreatableType<QDeclarativeAbstractAnimation>("Qt",4,7,"Animation",QDeclarativeAbstractAnimation::tr("Animation is an abstract class"));

    qmlRegisterCustomType<QDeclarativeListModel>("Qt", 4,7, "ListModel", new QDeclarativeListModelParser);
    qmlRegisterCustomType<QDeclarativePropertyChanges>("Qt", 4, 7, "PropertyChanges", new QDeclarativePropertyChangesParser);
    qmlRegisterCustomType<QDeclarativeConnections>("Qt", 4, 7, "Connections", new QDeclarativeConnectionsParser);
#endif
}
