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

#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qlist.h>
#include <qtextstream.h>
#include <qdir.h>

QT_BEGIN_NAMESPACE

class MakeItem;

class Configure
{
public:
    Configure( int& argc, char** argv );
    ~Configure();

    void parseCmdLine();
#if !defined(EVAL)
    void validateArgs();
#endif
    bool displayHelp();

    QString defaultTo(const QString &option);
    bool checkAvailability(const QString &part);
    void autoDetection();
    bool verifyConfiguration();

    void generateOutputVars();
#if !defined(EVAL)
    void generateHeaders();
    void generateBuildKey();
    void generateCachefile();
    void displayConfig();
    void buildQmake();
    void buildHostTools();
#endif
    void generateMakefiles();
    void appendMakeItem(int inList, const QString &item);
#if !defined(EVAL)
    void generateConfigfiles();
#endif
    void showSummary();
    void findProjects( const QString& dirName );
    QString firstLicensePath();

#if !defined(EVAL)
    bool showLicense(QString licenseFile);
    void readLicense();
#endif

    QString addDefine(QString def);

    enum ProjectType {
	App,
	Lib,
	Subdirs
    };

    ProjectType projectType( const QString& proFileName );
    bool isDone();
    bool isOk();
private:
    // Our variable dictionaries
    QMap<QString,QString> dictionary;
    QStringList licensedModules;
    QStringList allSqlDrivers;
    QStringList allConfigs;
    QStringList disabledModules;
    QStringList enabledModules;
    QStringList modules;
    QStringList disabledBuildParts;
//    QStringList sqlDrivers;
    QStringList configCmdLine;
    QStringList qmakeConfig;
    QStringList qtConfig;

    QStringList qmakeSql;
    QStringList qmakeSqlPlugins;

    QStringList qmakeStyles;
    QStringList qmakeStylePlugins;

    QStringList qmakeVars;
    QStringList qmakeDefines;
    //  makeList[0] for qt and qtmain
    //  makeList[1] for subdirs and libs
    //  makeList[2] for the rest
    QList<MakeItem*> makeList[3];
    QStringList qmakeIncludes;
    QStringList qmakeLibs;
    QString opensslLibs;
    QString psqlLibs;
    QString sybase;
    QString sybaseLibs;

    QMap<QString,QString> licenseInfo;
    QString outputLine;

    QTextStream outStream;
    QString sourcePath, buildPath;
    QDir sourceDir, buildDir;

    // Variables for usage output
    int optionIndent;
    int descIndent;
    int outputWidth;

    bool useUnixSeparators;
    QString fixSeparators(const QString &somePath, bool escape = false);
    QString escapeSeparators(const QString &somePath);
    bool filesDiffer(const QString &file1, const QString &file2);

    bool findFile(const QString &fileName);
    static QString findFileInPaths(const QString &fileName, const QString &paths);
#if !defined(EVAL)
    void reloadCmdLine();
    void saveCmdLine();
#endif

    void desc(const char *description, int startingAt = 0, int wrapIndent = 0);
    void desc(const char *option, const char *description, bool skipIndent = false, char fillChar = '.');
    void desc(const char *mark_option, const char *mark, const char *option, const char *description, char fillChar = '.');
    void applySpecSpecifics();
    static QString locateFile(const QString &fileName);
    static QString locateFileInPaths(const QString &fileName, const QStringList &paths);
};

class MakeItem
{
public:
    MakeItem( const QString &d, const QString &p, const QString &t, Configure::ProjectType qt )
	: directory( d ),
	  proFile( p ),
	  target( t ),
	  qmakeTemplate( qt )
    { }

    QString directory;
    QString proFile;
    QString target;
    Configure::ProjectType qmakeTemplate;
};


QT_END_NAMESPACE
