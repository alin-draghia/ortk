@echo off
@echo off

set my_dir=%~dp0%
cd /D %my_dir%

set INRIAPerson_dir="c:\Users\alin.draghia\Google Drive\scoala\Licenta\object-recognition-toolkit\datasets\INRIAPerson"
rmdir INRIAPerson
mklink /D INRIAPerson %INRIAPerson_dir%

pause