@echo off

SET COV=cov-int
SET PATH=C:\Apps\Coverity\bin;%PATH%;

IF EXIST %COV% rmdir /S /Q %COV%

cov-build --dir cov-int make 7 debug

pause