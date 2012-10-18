@echo off

@set ClientDestDir=.\Artifacts\Client\
@set ClientSrcDir=.\VNOC\Release

@echo ================	Build	=================
"%VS10PATH%\devenv.com" .\VNOC\VNOC.sln \build Release

@echo ================	Digital Sigin	================

@echo ================	Copy Artifacts	================
@mkdir "%ClientDestDir%"
@xcopy "%ClientSrcDir%" "%ClientDestDir%" /e/c/y
@echo ================	End~~	=================
