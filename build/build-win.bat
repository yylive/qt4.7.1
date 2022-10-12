set "vs_tools_param="

if not "%1"=="" (
	set "vs_tools_param=%1"
	goto vs_tools
)

if not "%VS90COMNTOOLS%"=="" (
	goto vs_tools
)

if not "%VS140COMNTOOLS%"=="" (
    set "vs_tools_param=vs2015"
	goto vs_tools
)

if not "%VS120COMNTOOLS%"=="" (
    set "vs_tools_param=vs2013"
	goto vs_tools
)

:vs_tools

if "%vs_tools_param%"=="vs2013" (
		set make_proj_func="qmake2.py"
	) else (
		if "%vs_tools_param%"=="vs2015" (
			set make_proj_func="qmake2_vs2015.py"
		)
	)

for %%I in ("%~dp0\..") do set QT=%%~fI
cd %QT%

call %QT%/bin/qtvars.bat vsvars %vs_tools_param%

cd %QT%
call qtminibuild.cmd %vs_tools_param%
cd %QT%/src
if "%vs_tools_param%"=="" ( call qmake -tp vc -r ) else ( call %make_proj_func% )

call devenv src.sln /build debug
call devenv src.sln /build release

cd %QT%/tools/designer
if not "%vs_tools_param%"=="" ( call %make_proj_func% )
call devenv designer.sln /build release

cd %QT%/tools/linguist
if not "%vs_tools_param%"=="" ( call %make_proj_func% )
call devenv linguist.sln /build release 

cd %QT%
call %QT%/build/exportbin.bat