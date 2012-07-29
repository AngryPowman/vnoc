@echo off

@xcopy /e/c/y "../../VisualLeakDetector/lib/win32" "%1" > nul
@xcopy /e/c/y "../../VisualLeakDetector/bin/win32" "%1" > nul
@xcopy /e/c/y "../../scintilla/bin" "%1" > nul

:gEnd
@echo "PreLink Event OK! "