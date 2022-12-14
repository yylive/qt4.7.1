/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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


#include <QtTest/QtTest>

#include <qcoreapplication.h>
#include <qdebug.h>
#include <qdiriterator.h>
#include <qfileinfo.h>
#include <qstringlist.h>

#if defined(Q_OS_VXWORKS)
#define Q_NO_SYMLINKS
#endif

#if defined(Q_OS_SYMBIAN)
// Open C in Symbian doesn't support symbolic links to directories
#define Q_NO_SYMLINKS_TO_DIRS
#endif

Q_DECLARE_METATYPE(QDirIterator::IteratorFlags)
Q_DECLARE_METATYPE(QDir::Filters)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QDirIterator : public QObject
{
    Q_OBJECT

public:
    tst_QDirIterator();
    virtual ~tst_QDirIterator();

private: // convenience functions
    QStringList createdDirectories;
    QStringList createdFiles;

    QDir currentDir;
    bool createDirectory(const QString &dirName)
    {
        if (currentDir.mkdir(dirName)) {
            createdDirectories.prepend(dirName);
            return true;
        }
        return false;
    }

    enum Cleanup { DoDelete, DontDelete };
    bool createFile(const QString &fileName, Cleanup cleanup = DoDelete)
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            if (cleanup == DoDelete)
                createdFiles << fileName;
            return true;
        }
        return false;
    }

    bool createLink(const QString &destination, const QString &linkName)
    {
        if (QFile::link(destination, linkName)) {
            createdFiles << linkName;
            return true;
        }
        return false;
    }

private slots:
    void iterateRelativeDirectory_data();
    void iterateRelativeDirectory();
    void iterateResource_data();
    void iterateResource();
    void stopLinkLoop();
    void engineWithNoIterator();
    void absoluteFilePathsFromRelativeIteratorPath();
    void recurseWithFilters() const;
    void longPath();
    void task185502_dirorder();
    void relativePaths();
    void qtbug15421_hiddenDirs_hiddenFiles();
};

tst_QDirIterator::tst_QDirIterator()
{
    QFile::remove("entrylist/entrylist1.lnk");
    QFile::remove("entrylist/entrylist2.lnk");
    QFile::remove("entrylist/entrylist3.lnk");
    QFile::remove("entrylist/entrylist4.lnk");
    QFile::remove("entrylist/directory/entrylist1.lnk");
    QFile::remove("entrylist/directory/entrylist2.lnk");
    QFile::remove("entrylist/directory/entrylist3.lnk");
    QFile::remove("entrylist/directory/entrylist4.lnk");

    createDirectory("entrylist");
    createDirectory("entrylist/directory");
    createFile("entrylist/file", DontDelete);
    createFile("entrylist/writable");
    createFile("entrylist/directory/dummy", DontDelete);

    createDirectory("recursiveDirs");
    createDirectory("recursiveDirs/dir1");
    createFile("recursiveDirs/textFileA.txt");
    createFile("recursiveDirs/dir1/aPage.html");
    createFile("recursiveDirs/dir1/textFileB.txt");

    createDirectory("foo");
    createDirectory("foo/bar");
    createFile("foo/bar/readme.txt");

#ifndef Q_NO_SYMLINKS
#  if defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
    // ### Sadly, this is a platform difference right now.
    createLink("entrylist/file", "entrylist/linktofile.lnk");
#    ifndef Q_NO_SYMLINKS_TO_DIRS
    createLink("entrylist/directory", "entrylist/linktodirectory.lnk");
#    endif
    createLink("entrylist/nothing", "entrylist/brokenlink.lnk");
#  else
    createLink("file", "entrylist/linktofile.lnk");
#    ifndef Q_NO_SYMLINKS_TO_DIRS
    createLink("directory", "entrylist/linktodirectory.lnk");
#    endif
    createLink("nothing", "entrylist/brokenlink.lnk");
#  endif
#endif

    createDirectory("qtbug15421_hiddenDirs_hiddenFiles");
    createFile("qtbug15421_hiddenDirs_hiddenFiles/normalFile");
    createFile("qtbug15421_hiddenDirs_hiddenFiles/.hiddenFile");
    createDirectory("qtbug15421_hiddenDirs_hiddenFiles/normalDirectory");
    createDirectory("qtbug15421_hiddenDirs_hiddenFiles/.hiddenDirectory");
    createFile("qtbug15421_hiddenDirs_hiddenFiles/normalDirectory/normalFile");
    createFile("qtbug15421_hiddenDirs_hiddenFiles/normalDirectory/.hiddenFile");
    createFile("qtbug15421_hiddenDirs_hiddenFiles/.hiddenDirectory/normalFile");
    createFile("qtbug15421_hiddenDirs_hiddenFiles/.hiddenDirectory/.hiddenFile");
    createDirectory("qtbug15421_hiddenDirs_hiddenFiles/normalDirectory/normalDirectory");
    createDirectory("qtbug15421_hiddenDirs_hiddenFiles/normalDirectory/.hiddenDirectory");
    createDirectory("qtbug15421_hiddenDirs_hiddenFiles/.hiddenDirectory/normalDirectory");
    createDirectory("qtbug15421_hiddenDirs_hiddenFiles/.hiddenDirectory/.hiddenDirectory");
}

tst_QDirIterator::~tst_QDirIterator()
{
    Q_FOREACH(QString fileName, createdFiles)
        QFile::remove(fileName);

    Q_FOREACH(QString dirName, createdDirectories)
        currentDir.rmdir(dirName);
}

void tst_QDirIterator::iterateRelativeDirectory_data()
{
    QTest::addColumn<QString>("dirName"); // relative from current path or abs
    QTest::addColumn<QDirIterator::IteratorFlags>("flags");
    QTest::addColumn<QDir::Filters>("filters");
    QTest::addColumn<QStringList>("nameFilters");
    QTest::addColumn<QStringList>("entries");

    QTest::newRow("no flags")
        << QString("entrylist") << QDirIterator::IteratorFlags(0)
        << QDir::Filters(QDir::NoFilter) << QStringList("*")
        << QString(
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
                  "entrylist/.,"
                   "entrylist/..,"
#endif
                   "entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/directory,"
#if !defined(Q_NO_SYMLINKS) && !defined(Q_NO_SYMLINKS_TO_DIRS)
                   "entrylist/linktodirectory.lnk,"
#endif
                   "entrylist/writable").split(',');

    QTest::newRow("NoDot")
        << QString("entrylist") << QDirIterator::IteratorFlags(0)
        << QDir::Filters(QDir::AllEntries | QDir::NoDot) << QStringList("*")
        << QString(
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
                   "entrylist/..,"
#endif
                   "entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/directory,"
#if !defined(Q_NO_SYMLINKS) && !defined(Q_NO_SYMLINKS_TO_DIRS)
                   "entrylist/linktodirectory.lnk,"
#endif
                   "entrylist/writable").split(',');

    QTest::newRow("NoDotDot")
        << QString("entrylist") << QDirIterator::IteratorFlags(0)
        << QDir::Filters(QDir::AllEntries | QDir::NoDotDot) << QStringList("*")
        << QString(
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
                  "entrylist/.,"
#endif
                   "entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/directory,"
#if !defined(Q_NO_SYMLINKS) && !defined(Q_NO_SYMLINKS_TO_DIRS)
                   "entrylist/linktodirectory.lnk,"
#endif
                   "entrylist/writable").split(',');

    QTest::newRow("NoDotAndDotDot")
        << QString("entrylist") << QDirIterator::IteratorFlags(0)
        << QDir::Filters(QDir::AllEntries | QDir::NoDotAndDotDot) << QStringList("*")
        << QString(
                   "entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/directory,"
#if !defined(Q_NO_SYMLINKS) && !defined(Q_NO_SYMLINKS_TO_DIRS)
                   "entrylist/linktodirectory.lnk,"
#endif
                   "entrylist/writable").split(',');

    QTest::newRow("QDir::Subdirectories | QDir::FollowSymlinks")
        << QString("entrylist") << QDirIterator::IteratorFlags(QDirIterator::Subdirectories | QDirIterator::FollowSymlinks)
        << QDir::Filters(QDir::NoFilter) << QStringList("*")
        << QString(
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
                   "entrylist/.,"
                   "entrylist/..,"
                   "entrylist/directory/.,"
                   "entrylist/directory/..,"
#endif
                   "entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/directory,"
                   "entrylist/directory/dummy,"
#if !defined(Q_NO_SYMLINKS) && !defined(Q_NO_SYMLINKS_TO_DIRS)
                   "entrylist/linktodirectory.lnk,"
#endif
                   "entrylist/writable").split(',');

    QTest::newRow("QDir::Subdirectories / QDir::Files")
        << QString("entrylist") << QDirIterator::IteratorFlags(QDirIterator::Subdirectories)
        << QDir::Filters(QDir::Files) << QStringList("*")
        << QString("entrylist/directory/dummy,"
                   "entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/writable").split(',');

    QTest::newRow("QDir::Subdirectories | QDir::FollowSymlinks / QDir::Files")
        << QString("entrylist") << QDirIterator::IteratorFlags(QDirIterator::Subdirectories | QDirIterator::FollowSymlinks)
        << QDir::Filters(QDir::Files) << QStringList("*")
        << QString("entrylist/file,"
#ifndef Q_NO_SYMLINKS
                   "entrylist/linktofile.lnk,"
#endif
                   "entrylist/directory/dummy,"
                   "entrylist/writable").split(',');
}

void tst_QDirIterator::iterateRelativeDirectory()
{
    QFETCH(QString, dirName);
    QFETCH(QDirIterator::IteratorFlags, flags);
    QFETCH(QDir::Filters, filters);
    QFETCH(QStringList, nameFilters);
    QFETCH(QStringList, entries);

    QDirIterator it(dirName, nameFilters, filters, flags);
    QStringList list;
    while (it.hasNext()) {
        QString next = it.next();

        QString fileName = it.fileName();
        QString filePath = it.filePath();
        QString path = it.path();

        QFileInfo info = it.fileInfo();

        QCOMPARE(path, dirName);
        QCOMPARE(next, filePath);

        QCOMPARE(info, QFileInfo(next));
        QCOMPARE(fileName, info.fileName());
        QCOMPARE(filePath, info.filePath());

        // Using canonical file paths for final comparison
        list << info.canonicalFilePath();
    }

    // The order of items returned by QDirIterator is not guaranteed.
    list.sort();

    QStringList sortedEntries;
    foreach(QString item, entries)
        sortedEntries.append(QFileInfo(item).canonicalFilePath());
    sortedEntries.sort();

    if (sortedEntries != list) {
        qDebug() << "EXPECTED:" << sortedEntries;
        qDebug() << "ACTUAL:  " << list;
    }

    QCOMPARE(list, sortedEntries);
}

void tst_QDirIterator::iterateResource_data()
{
    QTest::addColumn<QString>("dirName"); // relative from current path or abs
    QTest::addColumn<QDirIterator::IteratorFlags>("flags");
    QTest::addColumn<QDir::Filters>("filters");
    QTest::addColumn<QStringList>("nameFilters");
    QTest::addColumn<QStringList>("entries");

    QTest::newRow("invalid") << QString::fromLatin1(":/burpaburpa") << QDirIterator::IteratorFlags(0)
                             << QDir::Filters(QDir::NoFilter) << QStringList(QLatin1String("*"))
                             << QStringList();
    QTest::newRow(":/") << QString::fromLatin1(":/") << QDirIterator::IteratorFlags(0)
                               << QDir::Filters(QDir::NoFilter) << QStringList(QLatin1String("*"))
                               << QString::fromLatin1(":/entrylist").split(QLatin1String(","));
    QTest::newRow(":/entrylist") << QString::fromLatin1(":/entrylist") << QDirIterator::IteratorFlags(0)
                               << QDir::Filters(QDir::NoFilter) << QStringList(QLatin1String("*"))
                               << QString::fromLatin1(":/entrylist/directory,:/entrylist/file").split(QLatin1String(","));
    QTest::newRow(":/ recursive") << QString::fromLatin1(":/") << QDirIterator::IteratorFlags(QDirIterator::Subdirectories)
                                         << QDir::Filters(QDir::NoFilter) << QStringList(QLatin1String("*"))
                                         << QString::fromLatin1(":/entrylist,:/entrylist/directory,:/entrylist/directory/dummy,:/entrylist/file").split(QLatin1String(","));
}

void tst_QDirIterator::iterateResource()
{
    QFETCH(QString, dirName);
    QFETCH(QDirIterator::IteratorFlags, flags);
    QFETCH(QDir::Filters, filters);
    QFETCH(QStringList, nameFilters);
    QFETCH(QStringList, entries);

    QDirIterator it(dirName, nameFilters, filters, flags);
    QStringList list;
    while (it.hasNext())
        list << it.next();

    list.sort();
    QStringList sortedEntries = entries;
    sortedEntries.sort();

    if (sortedEntries != list) {
        qDebug() << "EXPECTED:" << sortedEntries;
        qDebug() << "ACTUAL:" << list;
    }

    QCOMPARE(list, sortedEntries);
}

void tst_QDirIterator::stopLinkLoop()
{
#ifdef Q_OS_WIN
    // ### Sadly, this is a platform difference right now.
    createLink(QDir::currentPath() + QLatin1String("/entrylist"), "entrylist/entrylist1.lnk");
    createLink("entrylist/.", "entrylist/entrylist2.lnk");
    createLink("entrylist/../entrylist/.", "entrylist/entrylist3.lnk");
    createLink("entrylist/..", "entrylist/entrylist4.lnk");
    createLink(QDir::currentPath() + QLatin1String("/entrylist"), "entrylist/directory/entrylist1.lnk");
    createLink("entrylist/.", "entrylist/directory/entrylist2.lnk");
    createLink("entrylist/../directory/.", "entrylist/directory/entrylist3.lnk");
    createLink("entrylist/..", "entrylist/directory/entrylist4.lnk");
#else
    createLink(QDir::currentPath() + QLatin1String("/entrylist"), "entrylist/entrylist1.lnk");
    createLink(".", "entrylist/entrylist2.lnk");
    createLink("../entrylist/.", "entrylist/entrylist3.lnk");
    createLink("..", "entrylist/entrylist4.lnk");
    createLink(QDir::currentPath() + QLatin1String("/entrylist"), "entrylist/directory/entrylist1.lnk");
    createLink(".", "entrylist/directory/entrylist2.lnk");
    createLink("../directory/.", "entrylist/directory/entrylist3.lnk");
    createLink("..", "entrylist/directory/entrylist4.lnk");
#endif

    QDirIterator it(QLatin1String("entrylist"), QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
    QStringList list;
    int max = 200;
    while (--max && it.hasNext())
        it.next();
    QVERIFY(max);

    // The goal of this test is only to ensure that the test above don't malfunction
}

class EngineWithNoIterator : public QFSFileEngine
{
public:
    EngineWithNoIterator(const QString &fileName)
        : QFSFileEngine(fileName)
    { }

    QAbstractFileEngineIterator *beginEntryList(QDir::Filters, const QStringList &)
    { return 0; }
};

class EngineWithNoIteratorHandler : public QAbstractFileEngineHandler
{
public:
    QAbstractFileEngine *create(const QString &fileName) const
    {
        return new EngineWithNoIterator(fileName);
    }
};

void tst_QDirIterator::engineWithNoIterator()
{
    EngineWithNoIteratorHandler handler;

    QDir("entrylist").entryList();
    QVERIFY(true); // test that the above line doesn't crash
}

void tst_QDirIterator::absoluteFilePathsFromRelativeIteratorPath()
{
    QDirIterator it("entrylist/", QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        it.next();
        QVERIFY(QFileInfo(it.filePath()).absoluteFilePath().contains("entrylist"));
    }
}

void tst_QDirIterator::recurseWithFilters() const
{
    QStringList nameFilters;
    nameFilters.append("*.txt");

    QDirIterator it("recursiveDirs/", nameFilters, QDir::Files,
                    QDirIterator::Subdirectories);

    QSet<QString> actualEntries;
    QSet<QString> expectedEntries;
    expectedEntries.insert(QString::fromLatin1("recursiveDirs/dir1/textFileB.txt"));
    expectedEntries.insert(QString::fromLatin1("recursiveDirs/textFileA.txt"));

    QVERIFY(it.hasNext());
    it.next();
    actualEntries.insert(it.fileInfo().filePath());
    QVERIFY(it.hasNext());
    it.next();
    actualEntries.insert(it.fileInfo().filePath());
    QVERIFY(actualEntries == expectedEntries);

    QVERIFY(!it.hasNext());
}

void tst_QDirIterator::longPath()
{
    QDir dir;
    dir.mkdir("longpaths");
    dir.cd("longpaths");

    QString dirName = "x";
    int n = 0;
    while (dir.exists(dirName) || dir.mkdir(dirName)) {
        ++n;
        dirName.append('x');
#if defined(Q_OS_WINCE) && defined(WINCE_BROKEN_ITERATE)
        // Some Windows CE devices/emulators are broken.
        // though one can create directories of length <= 217,
        // FindNextFile only reports entries until ~ 214.
        if (n >= 210)
            break;
#endif
    }

    QDirIterator it(dir.absolutePath(), QDir::NoDotAndDotDot|QDir::Dirs, QDirIterator::Subdirectories);
    int m = 0;
    while (it.hasNext()) {
        ++m;
        it.next();
    }

    QCOMPARE(n, m);

    dirName.chop(1);
    while (dirName.length() > 0 && dir.exists(dirName) && dir.rmdir(dirName)) {
        dirName.chop(1);
    }
    dir.cdUp();
    dir.rmdir("longpaths");
}

void tst_QDirIterator::task185502_dirorder()
{
    QDirIterator iterator("foo", QDirIterator::Subdirectories);
    while (iterator.hasNext() && iterator.next() != "foo/bar")
    { }

    QCOMPARE(iterator.filePath(), QString("foo/bar"));
    QCOMPARE(iterator.fileInfo().filePath(), QString("foo/bar"));
}

void tst_QDirIterator::relativePaths()
{
    QDirIterator iterator("*", QDirIterator::Subdirectories);
    while(iterator.hasNext()) {
        QCOMPARE(iterator.filePath(), QDir::cleanPath(iterator.filePath()));
    }
}

void tst_QDirIterator::qtbug15421_hiddenDirs_hiddenFiles()
{
    // In Unix it is easy to create hidden files, but in Windows it requires
    // a special call since hidden files need to be "marked" while in Unix
    // anything starting by a '.' is a hidden file.
    // For that reason this test is not run in Windows.
#if defined Q_OS_WIN || Q_OS_WINCE
    QSKIP("To create hidden files a special call is required in Windows.", SkipAll);
#else
    // Only files
    {
        int matches = 0;
        int failures = 0;
        QDirIterator di("qtbug15421_hiddenDirs_hiddenFiles", QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (di.hasNext()) {
            ++matches;
            QString filename = di.next();
            if (QFileInfo(filename).isDir())
                ++failures;    // search was only supposed to find files
        }
        QCOMPARE(matches, 6);
        QCOMPARE(failures, 0);
    }
    // Only directories
    {
        int matches = 0;
        int failures = 0;
        QDirIterator di("qtbug15421_hiddenDirs_hiddenFiles", QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (di.hasNext()) {
            ++matches;
            QString filename = di.next();
            if (!QFileInfo(filename).isDir())
                ++failures;    // search was only supposed to find files
        }
        QCOMPARE(matches, 6);
        QCOMPARE(failures, 0);
    }
#endif // Q_OS_WIN || Q_OS_WINCE
}

QTEST_MAIN(tst_QDirIterator)

#include "tst_qdiriterator.moc"

