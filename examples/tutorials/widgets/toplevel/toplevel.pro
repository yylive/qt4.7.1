SOURCES = main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/tutorials/widgets/toplevel
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS toplevel.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/tutorials/widgets/toplevel
INSTALLS += target sources
