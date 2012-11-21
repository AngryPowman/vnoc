@echo off 

@set ServerDestDir=.\Artifacts\Server\
@set ServerSrcDir=.\NetService\Release

@set ClientDestDir=.\Artifacts\Client\
@set ClientSrcDir=.\VNOC\Release

@echo ================	Build Client	=================
"%VS10PATH%\devenv.com" .\VNOC\VNOC.sln /rebuild "Release|Win32"

@echo .
@echo .
@echo .
@echo ================	Unit Test	================
"%ClientSrcDir%\testRunner.exe"
@echo .
@echo .
@echo .
@echo ================	Copy Artifacts	================

@mkdir "%ClientDestDir%"
@xcopy "%ClientSrcDir%" "%ClientDestDir%" /e/c/y

@echo .
@echo .
@echo .
@echo ================	End~~	=================
@exit /b 0
