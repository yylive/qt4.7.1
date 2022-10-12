TARGET  = qgif
include(../../qpluginbase.pri)

include(../../../gui/image/qgifhandler.pri)

QMAKE_LFLAGS_RELEASE += /BASE:0x67C00000

SOURCES += $$PWD/main.cpp

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/imageformats
target.path += $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

symbian:TARGET.UID3=0x2001E61A
