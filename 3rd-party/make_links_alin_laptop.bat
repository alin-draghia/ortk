@echo off

set my_dir=%~dp0%
cd /D %my_dir%


rmdir opencv
rmdir eigen
rmdir dlib
rmdir poco
rmdir vlfeat
rmdir python

mklink /D opencv d:\dev\opencv-2.4.8\opencv\my-build-x64\install
mklink /D eigen d:\dev\eigen\eigen-eigen-6b38706d90a9
mklink /D dlib d:\dev\dlib-18.6
mklink /D poco d:\dev\poco-develop\my_build\install
mklink /D vlfeat d:\dev\vlfeat-master
mklink /D python c:\Users\Alin\AppData\Local\Enthought\Canopy\App\appdata\canopy-1.3.0.1715.win-x86_64