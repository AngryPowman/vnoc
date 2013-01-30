@echo off
setlocal EnableDelayedExpansion

@xcopy /f/c/y "Resource/*" "../%1" > nul

:gEnd
@echo "Post Dispatch OK!"