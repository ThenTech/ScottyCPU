@echo off

SET SCOTTY=bin\ScottyCPU.exe
SET LOGS=bin\DrMemoryOutput

:START
IF EXIST %SCOTTY% (
	IF EXIST %LOGS% rmdir /S /Q %LOGS%
	mkdir %LOGS%

	call C:\Apps\DrMemory\bin64\drmemory -no_follow_children -logdir %LOGS% -- %SCOTTY% -d > bin\output.txt
) else (
	call make 7 debug-extra
	CLS
	GOTO :START
)

exit 0

