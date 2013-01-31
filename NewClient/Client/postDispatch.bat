@echo off
setlocal EnableDelayedExpansion

@xcopy /f/c/y "Resource/*" "../%1" > nul
@xcopy /f/c/y "../../ThirdLibs/VisualLeakDetector/bin/Win32" "../%1" > nul

:gEnd
@echo "Post Dispatch OK!"