@echo off
setlocal EnableDelayedExpansion

@if "%1"=="RELEASE" goto gRelease
cd
xcopy /e/c/y "../VisualLeakDetector/bin/win32" "Debug" > nul

goto gEnd

:gRelease
@xcopy /e/c/y "../VisualLeakDetector/bin/win32" "Release" > nul

:gEnd
@echo "Post Dispatch OK!"