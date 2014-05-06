@echo off
@echo off

set my_dir=%~dp0%
cd /D %my_dir%


rmdir object-recognition-toolkit\include
mklink /D object-recognition-toolkit\include %cd%\..\include

rmdir object-recognition-toolkit\src
mklink /D object-recognition-toolkit\src %cd%\..\src

rmdir 3rd-party
mklink /D 3rd-party %cd%\..\3rd-party

rmdir bin
mklink /D bin %cd%\..\bin

rmdir lib
mklink /D lib %cd%\..\lib

rmdir datasets
mklink /D datasets ..\datasets

pause