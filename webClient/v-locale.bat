@echo off
cd %cd%
color 18

:Start
  set input=
  echo -------------------------------------------------------------------------------
  echo Current Directory: %cd%
  echo Choose one option:
  echo   1; Make / Update *.POT template
  echo   2; Create        *.PO file
  echo   3; Delete        *.PO file
  echo   4; Update        *.PO files
  echo   5; Show          *.PO list
  echo   6; Color Settings
  echo   7; Exit
  echo -------------------------------------------------------------------------------

  set /p input=Option Index: 
  if %input% EQU 1 goto Template
  if %input% EQU 2 goto Generate
  if %input% EQU 3 goto Delete
  if %input% EQU 4 goto Update
  if %input% EQU 5 goto ShowList
  if %input% EQU 6 goto ColorSetting
  if %input% EQU 7 exit else echo Invalid Input

:Template
  setlocal ENABLEDELAYEDEXPANSION
  for /f "delims=" %%i in ('"dir /A /S /B /N /ON *.php"') do (
    set file=%%~fi
    set file=!file:%cd%\=!
    echo !file! >> v-lang/php.lst
  )
  setlocal DISABLEDELAYEDEXPANSION
  xgettext.exe -k__ -f v-lang/php.lst -o v-lang/VNOC.pot --from-code=utf-8
  del /F /Q v-lang\php.lst
  goto Done

:Generate
  set /p code=Please input locale setting: 
  msginit -l %code%.UTF-8 -i v-lang/VNOC.pot -o v-lang/%code%.po
  goto List

:Delete
  set /p file=Please input locale name: 
  del /F /Q v-lang\%file%.po
  goto List

:Update
  for /f %%i in ('"type v-lang\list.txt"') do (
    msgmerge -U -q v-lang/%%i.po v-lang/VNOC.pot
    del /F /Q v-lang\%%i.po~
  )
  goto List

:ShowList
  type v-lang\list.txt
  echo.
  goto Done

:List
  if exist v-lang\list.txt del /F /Q v-lang\list.txt
  setlocal ENABLEDELAYEDEXPANSION
  for /f %%i in ('"dir /A /S /B /N /ON v-lang\*.po"') do (
    set file=%%~fi
    set file=!file:%cd%\v-lang\=!
    set file=!file:.po=!
    echo !file! >> v-lang/list.txt
  )
  setlocal DISABLEDELAYEDEXPANSION
  goto Done

:ColorSetting
  echo Default Color : [BACKCOLOR] = 1               [FORECOLOR] = 8
  echo.
  echo [BACKCOLOR]
  echo   [0] Black           [1] Blue            [2] Green           [3] Light Green
  echo   [4] Red             [5] Purple          [6] Yellow          [7] White
  set /p back=Please select backcolor: 
  echo [FORECOLOR]
  echo   [8] Gray          [9] Pale Blue     [A] Pale Green    [B] Pale Light Green
  echo   [C] Pale Red      [D] Pale Purple   [E] Pale Yellow   [F] Light White
  set /p fore=Please select forecolor: 
  color %back%%fore%
  goto Done

:Done
  echo Done...
  echo Press any key to countinue...
  pause > nul.
  cls
  goto Start