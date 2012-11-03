@echo off

@xcopy /e/c/y "../../VisualLeakDetector/bin/%2" "%1" > nul
@xcopy /e/c/y "../../scintilla/bin" "%1" > nul

:gEnd
@echo "PreLink Event OK! "