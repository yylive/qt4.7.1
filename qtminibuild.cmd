@echo off
rem
rem configure Qt
rem

rem config for vs2015
if not "%1" == "vs2015" goto ENDVS2015
configure.exe -opensource  -confirm-license -qt-libjpeg -qt-zlib -qt-libpng -qt-libjpeg -qt-gif -no-libtiff -no-libmng -nomake examples -nomake demos -nomake doc -no-plugin-manifests -no-exceptions -no-rtti -no-qt3support -no-openssl -no-opengl -no-multimedia -no-3dnow -no-native-gestures -no-style-motif -no-style-cde -no-style-cleanlooks -no-style-plastique -no-sql-sqlite -no-dbus -platform win32-msvc2010
:ENDVS2015

rem config for vs2013
if not "%1" == "vs2013" goto ENDVS2013
configure.exe -opensource  -confirm-license -qt-libjpeg -qt-zlib -qt-libpng -qt-libjpeg -qt-gif -no-libtiff -no-libmng -nomake examples -nomake demos -nomake doc -no-plugin-manifests -no-exceptions -no-rtti -no-qt3support -no-openssl -no-opengl -no-multimedia -no-3dnow -no-native-gestures -no-style-motif -no-style-cde -no-style-cleanlooks -no-style-plastique -no-sql-sqlite -no-dbus -platform win32-msvc2010
:ENDVS2013

rem config for vs2008
if not "%1" == "" goto ENDVS2008
configure.exe -opensource  -confirm-license -qt-libjpeg -qt-zlib -qt-libpng -qt-libjpeg -qt-gif -no-libtiff -no-libmng -nomake examples -nomake demos -nomake doc -no-plugin-manifests -no-exceptions -no-rtti -no-qt3support -no-openssl -no-opengl -no-multimedia -no-3dnow -no-native-gestures -no-style-motif -no-style-cde -no-style-cleanlooks -no-style-plastique -no-sql-sqlite -no-dbus -platform win32-msvc2008
:ENDVS2008