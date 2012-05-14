@echo off
dir *.php /S /B > locale_PHPlist.lst
xgettext.exe -f locale_PHPlist.lst -o VNOC.pot --from-code=unicode
del /F locale_PHPlist.lst
echo Succeed Making *.pot file,
echo Press to open *.pot file...
pause.
start notepad VNOC.pot