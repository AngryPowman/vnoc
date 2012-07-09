@echo off
setlocal EnableDelayedExpansion

@if "%1"=="RELEASE" goto gRelease
cd
xcopy /e/c/y "../VisualLeakDetector/bin/win32" "Debug" > nul
xcopy /e/c/y "dll" "Debug" > nul

goto gEnd

:gRelease
@xcopy /e/c/y "../VisualLeakDetector/bin/win32" "Release" > nul
xcopy /e/c/y "dll" "Release" > nul

:gEnd
@echo "Post Dispatch OK!"