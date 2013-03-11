@echo off 

@set ServerDestDir=.\Artifacts\Server\
@set ServerSrcDir=.\NetService\Release

@set ClientDestDir=.\Artifacts\Client\
@set ClientSrcDir=.\Client\Release
@set CppTestDir=.\Client\CppTest

@echo ================	Build Client	=================
"%VS11PATH%\devenv.com" .\Client\VNOC.sln /rebuild "Release|Win32"
if ERRORLEVEL 1 exit /b 1
"%VS11PATH%\devenv.com" .\Client\VNOC.sln /rebuild "CppTest|Win32"
if ERRORLEVEL 1 exit /b 1
@echo .
@echo .
@echo .
@echo ================	Unit Test	================
"%CppTestDir%\Client.exe"
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