@echo off
@cd ..

@if "%1"=="RELEASE" goto gRelease
@xcopy /e/c/y "../VisualLeakDetector/bin/win32" "Debug" > nul
@xcopy /e/c/y "../scintilla/bin" "Debug" > nul
goto gEnd

:gRelease
@xcopy /e/c/y "../scintilla/bin" "Release" > nul

:gEnd
@echo "PreLink Event OK! "