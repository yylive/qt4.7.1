set rootDir=%~dp0

echo %rootDir%

XCOPY  /Y /S "%rootDir%*.*" "d:\Qt\4.7.1\"

del "d:\Qt\4.7.1\mergeqt.bat"
