@echo off
setlocal EnableDelayedExpansion

@xcopy /f/c/y "%1/*.dll" "../%1" > nul
@xcopy /f/c/y "%1/*.manifest" "../%1" > nul
@xcopy /f/c/y "config/*.xml" "../%1" > nul
@mkdir "../%1/WinRes" > nul
@xcopy /f/c/y "res/*.xml" "../%1/WinRes" > nul

:gEnd
@echo "Post Dispatch OK!"