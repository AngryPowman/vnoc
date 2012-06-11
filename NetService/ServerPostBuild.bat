@echo off
@cd ..
setlocal EnableDelayedExpansion

@if "%1"=="RELEASE" goto gRelease
;

goto gEnd

:gRelease
; 

:gEnd
@echo "Post Dispatch OK!"