@echo off

echo %cd%

rmdir object-recognition-toolkit\include
mklink /D object-recognition-toolkit\include %cd%\..\include

rmdir object-recognition-toolkit\src
mklink /D object-recognition-toolkit\src %cd%\..\src

rmdir bin
mklink /D bin %cd%\..\bin

rmdir 3rd-party
mklink /D 3rd-party %cd%\..\3rd-party


pause