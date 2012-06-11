@echo off
@cd ..

@if "%1"=="RELEASE" goto gRelease
goto gEnd

:gRelease

:gEnd
@echo "PreLink Event OK!"