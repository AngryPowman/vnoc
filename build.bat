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
@echo ================	Build Client	=================
"%VS10PATH%\devenv.com" .\VNOC\VNOC.sln /rebuild "Release|Win32"

@echo .
@echo .
@echo .
@echo ================	Digital Sigin	================

@echo .
@echo .
@echo .
@echo ================	Copy Artifacts	================
@mkdir "%ServerDestDir%"
@xcopy "%ServerSrcDir%" "%ServerDestDir%" /e/c/y

@mkdir "%ClientDestDir%"
@xcopy "%ClientSrcDir%" "%ClientDestDir%" /e/c/y

@echo .
@echo .
@echo .
@echo ================	End~~	=================
@exit /b 0
