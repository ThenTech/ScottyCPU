@echo off

SET SCOTTY=bin\ScottyCPU.exe

:START
IF EXIST %SCOTTY% (
	call %SCOTTY% -d
) else (
	call make
	CLS
	GOTO :START
)

exit 0
