TARGET  = qico
include(../../qpluginbase.pri)

QTDIR_build:REQUIRES = "!contains(QT_CONFIG, no-ico)"

QMAKE_LFLAGS_RELEASE += /BASE:0x67B00000

HEADERS += qicohandler.h
SOURCES += main.cpp \
           qicohandler.cpp

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/imageformats
target.path += $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

symbian:TARGET.UID3=0x2001E616
