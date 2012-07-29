@echo off
setlocal EnableDelayedExpansion

@xcopy /f/c/y "%1/*.dll" "../%1" > nul
@xcopy /f/c/y "config/*.xml" "../%1" > nul

:gEnd
@echo "Post Dispatch OK!"