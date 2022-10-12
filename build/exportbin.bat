set rootDir=%~dp0..

echo %rootDir%

set collectRootDir="%rootDir%\artifacts_tmp"
set packageRootDir="%rootDir%\artifacts"

rd %packageRootDir% /s /q
rd %collectRootDir% /s /q

md %collectRootDir%
md %packageRootDir%

md "%collectRootDir%\bin"
md "%collectRootDir%\bin\release"
md "%collectRootDir%\bin\release\imageformats"
COPY "%rootDir%\bin\lrelease.exe" "%collectRootDir%\bin\release\lrelease.exe"   /Y 
COPY "%rootDir%\bin\phonon4.dll" "%collectRootDir%\bin\release\phonon4.dll"   /Y 
COPY "%rootDir%\bin\QtCore4.dll" "%collectRootDir%\bin\release\QtCore4.dll"   /Y 
COPY "%rootDir%\bin\QtGui4.dll" "%collectRootDir%\bin\release\QtGui4.dll"   /Y 
COPY "%rootDir%\bin\QtNetwork4.dll" "%collectRootDir%\bin\release\QtNetwork4.dll"   /Y 
COPY "%rootDir%\bin\QtScript4.dll" "%collectRootDir%\bin\release\QtScript4.dll"   /Y 
COPY "%rootDir%\bin\QtWebKit4.dll" "%collectRootDir%\bin\release\QtWebKit4.dll"   /Y 
COPY "%rootDir%\bin\QtXml4.dll" "%collectRootDir%\bin\release\QtXml4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qgif4.dll" "%collectRootDir%\bin\release\imageformats\qgif4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qjpeg4.dll" "%collectRootDir%\bin\release\imageformats\qjpeg4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qico4.dll" "%collectRootDir%\bin\release\imageformats\qico4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qwebp4.dll" "%collectRootDir%\bin\release\imageformats\qwebp4.dll"   /Y 

md "%collectRootDir%\bin"
md "%collectRootDir%\bin\debug"
md "%collectRootDir%\bin\debug\imageformats"
COPY "%rootDir%\bin\phonond4.dll" "%collectRootDir%\bin\debug\phonond4.dll"   /Y 
COPY "%rootDir%\bin\QtCored4.dll" "%collectRootDir%\bin\debug\QtCored4.dll"   /Y 
COPY "%rootDir%\bin\QtGuid4.dll" "%collectRootDir%\bin\debug\QtGuid4.dll"   /Y 
COPY "%rootDir%\bin\QtNetworkd4.dll" "%collectRootDir%\bin\debug\QtNetworkd4.dll"   /Y 
COPY "%rootDir%\bin\QtScriptd4.dll" "%collectRootDir%\bin\debug\QtScriptd4.dll"   /Y 
COPY "%rootDir%\bin\QtWebKitd4.dll" "%collectRootDir%\bin\debug\QtWebKitd4.dll"   /Y 
COPY "%rootDir%\bin\QtXmld4.dll" "%collectRootDir%\bin\debug\QtXmld4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qgifd4.dll" "%collectRootDir%\bin\debug\imageformats\qgifd4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qjpegd4.dll" "%collectRootDir%\bin\debug\imageformats\qjpegd4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qicod4.dll" "%collectRootDir%\bin\debug\imageformats\qicod4.dll"   /Y 
COPY "%rootDir%\plugins\imageformats\qwebpd4.dll" "%collectRootDir%\bin\debug\imageformats\qwebpd4.dll"   /Y 

md "%collectRootDir%\lib"
md "%collectRootDir%\lib\release"
md "%collectRootDir%\lib\release\imageformats"
COPY "%rootDir%\lib\phonon4.lib" "%collectRootDir%\lib\release\phonon4.lib"   /Y 
COPY "%rootDir%\lib\QtCore4.lib" "%collectRootDir%\lib\release\QtCore4.lib"   /Y 
COPY "%rootDir%\lib\QtGui4.lib" "%collectRootDir%\lib\release\QtGui4.lib"   /Y 
COPY "%rootDir%\lib\QtNetwork4.lib" "%collectRootDir%\lib\release\QtNetwork4.lib"   /Y 
COPY "%rootDir%\lib\QtScript4.lib" "%collectRootDir%\lib\release\QtScript4.lib"   /Y 
COPY "%rootDir%\lib\QtWebKit4.lib" "%collectRootDir%\lib\release\QtWebKit4.lib"   /Y 
COPY "%rootDir%\lib\QtXml4.lib" "%collectRootDir%\lib\release\QtXml4.lib"   /Y 
COPY "%rootDir%\lib\QAxContainer.lib" "%collectRootDir%\lib\release\QAxContainer.lib"   /Y 
COPY "%rootDir%\lib\qtmain.lib" "%collectRootDir%\lib\release\qtmain.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qgif4.lib" "%collectRootDir%\lib\release\imageformats\qgif4.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qjpeg4.lib" "%collectRootDir%\lib\release\imageformats\qjpeg4.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qico4.lib" "%collectRootDir%\lib\release\imageformats\qico4.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qwebp4.lib" "%collectRootDir%\lib\release\imageformats\qwebp4.lib"   /Y 

md "%collectRootDir%\lib"
md "%collectRootDir%\lib\debug"
md "%collectRootDir%\lib\debug\imageformats"
COPY "%rootDir%\lib\phonond4.lib" "%collectRootDir%\lib\debug\phonond4.lib"   /Y 
COPY "%rootDir%\lib\QtCored4.lib" "%collectRootDir%\lib\debug\QtCored4.lib"   /Y 
COPY "%rootDir%\lib\QtGuid4.lib" "%collectRootDir%\lib\debug\QtGuid4.lib"   /Y 
COPY "%rootDir%\lib\QtNetworkd4.lib" "%collectRootDir%\lib\debug\QtNetworkd4.lib"   /Y 
COPY "%rootDir%\lib\QtScriptd4.lib" "%collectRootDir%\lib\debug\QtScriptd4.lib"   /Y 
COPY "%rootDir%\lib\QtWebKitd4.lib" "%collectRootDir%\lib\debug\QtWebKitd4.lib"   /Y 
COPY "%rootDir%\lib\QtXmld4.lib" "%collectRootDir%\lib\debug\QtXmld4.lib"   /Y 
COPY "%rootDir%\lib\QAxContainerd.lib" "%collectRootDir%\lib\debug\QAxContainerd.lib"   /Y 
COPY "%rootDir%\lib\qtmaind.lib" "%collectRootDir%\lib\debug\qtmaind.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qgifd4.lib" "%collectRootDir%\lib\debug\imageformats\qgifd4.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qjpegd4.lib" "%collectRootDir%\lib\debug\imageformats\qjpegd4.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qicod4.lib" "%collectRootDir%\lib\debug\imageformats\qicod4.lib"   /Y 
COPY "%rootDir%\plugins\imageformats\qwebpd4.lib" "%collectRootDir%\lib\debug\imageformats\qwebpd4.lib"   /Y 

md "%collectRootDir%\pdb"
md "%collectRootDir%\pdb\release"
md "%collectRootDir%\pdb\release\imageformats"
COPY "%rootDir%\bin\lrelease.pdb" "%collectRootDir%\pdb\release\lrelease.pdb"   /Y 
COPY "%rootDir%\lib\phonon4.pdb" "%collectRootDir%\pdb\release\phonon4.pdb"   /Y 
COPY "%rootDir%\lib\QtCore4.pdb" "%collectRootDir%\pdb\release\QtCore4.pdb"   /Y 
COPY "%rootDir%\lib\QtGui4.pdb" "%collectRootDir%\pdb\release\QtGui4.pdb"   /Y 
COPY "%rootDir%\lib\QtNetwork4.pdb" "%collectRootDir%\pdb\release\QtNetwork4.pdb"   /Y 
COPY "%rootDir%\lib\QtScript4.pdb" "%collectRootDir%\pdb\release\QtScript4.pdb"   /Y 
COPY "%rootDir%\lib\QtWebKit4.pdb" "%collectRootDir%\pdb\release\QtWebKit4.pdb"   /Y 
COPY "%rootDir%\lib\QtXml4.pdb" "%collectRootDir%\pdb\release\QtXml4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qgif4.pdb" "%collectRootDir%\pdb\release\imageformats\qgif4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qjpeg4.pdb" "%collectRootDir%\pdb\release\imageformats\qjpeg4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qico4.pdb" "%collectRootDir%\pdb\release\imageformats\qico4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qwebp4.pdb" "%collectRootDir%\pdb\release\imageformats\qwebp4.pdb"   /Y

md "%collectRootDir%\pdb"
md "%collectRootDir%\pdb\debug"
md "%collectRootDir%\pdb\debug\imageformats"
COPY "%rootDir%\lib\phonond4.pdb" "%collectRootDir%\pdb\debug\phonond4.pdb"   /Y 
COPY "%rootDir%\lib\QtCored4.pdb" "%collectRootDir%\pdb\debug\QtCored4.pdb"   /Y 
COPY "%rootDir%\lib\QtGuid4.pdb" "%collectRootDir%\pdb\debug\QtGuid4.pdb"   /Y 
COPY "%rootDir%\lib\QtNetworkd4.pdb" "%collectRootDir%\pdb\debug\QtNetworkd4.pdb"   /Y 
COPY "%rootDir%\lib\QtScriptd4.pdb" "%collectRootDir%\pdb\debug\QtScriptd4.pdb"   /Y 
COPY "%rootDir%\lib\QtWebKitd4.pdb" "%collectRootDir%\pdb\debug\QtWebKitd4.pdb"   /Y 
COPY "%rootDir%\lib\QtXmld4.pdb" "%collectRootDir%\pdb\debug\QtXmld4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qgifd4.pdb" "%collectRootDir%\pdb\debug\imageformats\qgifd4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qjpegd4.pdb" "%collectRootDir%\pdb\debug\imageformats\qjpegd4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qicod4.pdb" "%collectRootDir%\pdb\debug\imageformats\qicod4.pdb"   /Y 
COPY "%rootDir%\plugins\imageformats\qwebpd4.pdb" "%collectRootDir%\pdb\debug\imageformats\qwebpd4.pdb"   /Y

md "%collectRootDir%\qtpatch"
md "%collectRootDir%\qtpatch\bin"
md "%collectRootDir%\qtpatch\lib"
COPY "%rootDir%\bin\qmake.exe" "%collectRootDir%\qtpatch\bin\qmake.exe"   /Y 
COPY "%rootDir%\bin\moc.exe" "%collectRootDir%\qtpatch\bin\moc.exe"   /Y 
COPY "%rootDir%\bin\qmake2_vs2015.py" "%collectRootDir%\qtpatch\bin\qmake2_vs2015.py"   /Y 
COPY "%rootDir%\bin\qtvars.bat" "%collectRootDir%\qtpatch\bin\qtvars.bat"   /Y 
COPY "%rootDir%\build\mergeqt.bat" "%collectRootDir%\qtpatch\mergeqt.bat"   /Y 
XCOPY  /Y/E "%rootDir%\mkspecs\win32-msvc2008\*.*" "%collectRootDir%\qtpatch\mkspecs\win32-msvc2008\"
XCOPY  /Y/E "%rootDir%\mkspecs\win32-msvc2010\*.*" "%collectRootDir%\qtpatch\mkspecs\win32-msvc2010\"
XCOPY  /Y/E "%collectRootDir%\bin\release\*.*" "%collectRootDir%\qtpatch\bin\"
XCOPY  /Y/E "%collectRootDir%\lib\release\*.*" "%collectRootDir%\qtpatch\lib\"
XCOPY  /Y/E "%collectRootDir%\pdb\release\*.*" "%collectRootDir%\qtpatch\lib\"

python "%rootDir%\build\package.py" "%collectRootDir%\bin"  "%packageRootDir%\bin.tar.gz" "bin"
python "%rootDir%\build\package.py" "%collectRootDir%\lib"  "%packageRootDir%\dev.tar.gz" "lib"
python "%rootDir%\build\package.py" "%collectRootDir%\pdb"  "%packageRootDir%\pdb.tar.gz" "pdb"
python "%rootDir%\build\package.py" "%collectRootDir%\qtpatch"  "%packageRootDir%\qtpatch.tar.gz" "qtpatch"
