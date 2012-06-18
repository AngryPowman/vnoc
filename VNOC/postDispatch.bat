@echo off
@cd ..
setlocal EnableDelayedExpansion

@if "%1"=="RELEASE" goto gRelease
@xcopy /f/y "VNOC/config.xml" "Debug" > nul
@xcopy /f/y "VNOC/config.xml" "../Output/Debug" > nul
@xcopy /f/y "Debug/VNOC.exe" "../Output/Debug" > nul
@xcopy /f/y "Debug/Scintilla.dll" "../Output/Debug" > nul
goto gEnd

:gRelease
@xcopy /f/y "VNOC/config.xml" "Release" > nul
@xcopy /f/y "VNOC/config.xml" "../Output/Release" > nul
@xcopy /f/y "Release/VNOC.exe" "../Output/Release" > nul
@xcopy /f/y "Release/Scintilla.dll" "../Output/Release" > nul

:gEnd
@echo "Post Dispatch OK!"