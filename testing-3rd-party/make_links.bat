@echo off

echo %cd%

rmdir 3rd-party
mklink /D 3rd-party %cd%\..\3rd-party


pause