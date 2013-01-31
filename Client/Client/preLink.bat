@echo off

@xcopy /e/c/y "../../ThirdLibs/VisualLeakDetector/bin/%2" "%1" > nul
@xcopy /e/c/y "../../ThirdLibs/scintilla/bin" "%1" > nul

:gEnd
@echo "PreLink Event OK! "