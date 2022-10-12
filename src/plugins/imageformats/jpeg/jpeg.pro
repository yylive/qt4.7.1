TARGET  = qjpeg
include(../../qpluginbase.pri)

QTDIR_build:REQUIRES = "!contains(QT_CONFIG, no-jpeg)"

QMAKE_LFLAGS_RELEASE += /BASE:0x67A00000

include(../../../gui/image/qjpeghandler.pri)
SOURCES += main.cpp

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/imageformats
target.path += $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target
