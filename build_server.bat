@echo off

@set ServerDestDir=.\Artifacts\Server\
@set ServerSrcDir=.\NetService\Release

@set ClientDestDir=.\Artifacts\Client\
@set ClientSrcDir=.\VNOC\Release

@echo .
@echo .
@echo .
@echo ================	Build Server	=================
"%VS11PATH%\devenv.com" .\NetService\server.sln /rebuild "Release"

@echo .
@echo .
@echo .
@echo ================	Unit Test	=================
@echo "%ServerSrcDir%\testRunner.exe"
"%ServerSrcDir%\testRunner.exe"

@echo .
@echo .
@echo .
@echo ================	Copy Artifacts	================
@mkdir "%ServerDestDir%"
@xcopy "%ServerSrcDir%" "%ServerDestDir%" /e/c/y

@echo .
@echo .
@echo .
@echo ================	End~~	=================
@exit /b 0
