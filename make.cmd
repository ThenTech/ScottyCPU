@echo off
TITLE Run Makefile with a given compiler

SET USE_MAKE=
SET USE_ARG=

SET MSVC_MAKE=nmake.exe
SET MSVC_PATH_1="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"
SET MSVC_PATH_2=
for /f "delims=" %%a in ('where %MSVC_MAKE%') do @set MSVC_PATH_2="%%a"

SET MINGW_MAKE=mingw32-make.exe
SET MINGW_PATH_1="C:\Apps\Qt\Tools\mingw492_32\bin\"
SET MINGW_PATH_2="C:\Apps\Qt\Tools\mingw530_32\bin\"
SET MINGW_PATH_3="D:\Apps\Qt\Tools\mingw492_32\bin\"
SET MINGW_PATH_4="D:\Apps\Qt\Tools\mingw530_32\bin\"
SET MINGW_PATH_5=
for /f "delims=" %%a in ('where %MINGW_MAKE%') do @set MINGW_PATH_5="%%a"
SET MINGW_PATH_6="C:\Program Files (x86)\Qt\Tools\mingw530_32\bin\"
SET MINGW_PATH_7="C:\Program Files (x86)\Qt\Tools\mingw492_32\bin\"

CLS

if [%1]==[] (
	echo Supported compilers:
	IF EXIST %MSVC_PATH_1% echo 1: MSVC in default location
	IF EXIST %MSVC_PATH_2% echo 2: MSVC in PATH
	
	IF EXIST %MINGW_PATH_1% echo 3: MinGW 4.92 in C:\Apps\Qt\Tools
	IF EXIST %MINGW_PATH_2% echo 4: MinGW 5.30 in C:\Apps\Qt\Tools
	IF EXIST %MINGW_PATH_3% echo 5: MinGW 4.92 in D:\Apps\Qt\Tools
	IF EXIST %MINGW_PATH_4% echo 6: MinGW 5.30 in D:\Apps\Qt\Tools
	IF EXIST %MINGW_PATH_5% echo 7: MinGW in PATH
	IF EXIST %MINGW_PATH_6% echo 8: MinGW 4.92 in default location
	IF EXIST %MINGW_PATH_7% echo 9: MinGW 5.30 in default location

	SET /p Test= Enter number for the compiler to use: 
	SET /a COMPILER_ID=Test
) else (
	SET /a COMPILER_ID=%1
)	


IF NOT %COMPILER_ID% == 0 (
	IF %COMPILER_ID% == 1 (
		call %MSVC_PATH_1%
		SET USE_MAKE=%MSVC_MAKE%
		goto :Compile
	)
	IF %COMPILER_ID% == 2 (
		SET USE_MAKE=%MSVC_MAKE%
		goto :Compile
	)
	
	IF %COMPILER_ID% == 3 (
		SET USE_MAKE=%MINGW_PATH_1%
		goto :MinGWCompile
	)
	IF %COMPILER_ID% == 4 (
		SET USE_MAKE=%MINGW_PATH_2%
		goto :MinGWCompile
	)
	IF %COMPILER_ID% == 5 (
		SET USE_MAKE=%MINGW_PATH_3%
		goto :MinGWCompile
	)
	IF %COMPILER_ID% == 6 (
		SET USE_MAKE=%MINGW_PATH_4%
		goto :MinGWCompile
	)
	IF %COMPILER_ID% == 7 (
		SET USE_MAKE=%MINGW_PATH_5%
		goto :Compile
	)
	IF %COMPILER_ID% == 8 (
		SET USE_MAKE=%MINGW_PATH_6%
		goto :MinGWCompile
	)
	IF %COMPILER_ID% == 9 (
		SET USE_MAKE=%MINGW_PATH_7%
		goto :MinGWCompile
	)
	
	echo Invalid argument.
	GOTO :END
)

:MinGWCompile
setlocal
SET PATH=%USE_MAKE:"=%;%PATH%
SET USE_MAKE=%USE_MAKE%%MINGW_MAKE%

:Compile
IF [%2]==[] (
	SET /p USE_ARG= "Enter Makefile argument (debug, release, clean...): "
) else (
	SET USE_ARG=%2
)

echo Calling:
echo %USE_MAKE% %USE_ARG%
echo.

%USE_MAKE% %USE_ARG%

:END
echo.
echo Ended with errorlevel %ERRORLEVEL%
pause
