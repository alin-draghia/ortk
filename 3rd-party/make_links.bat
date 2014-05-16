@echo off

set my_dir=%~dp0%
cd /D %my_dir%

rmdir opencv
mklink /D opencv D:\SDK\OpenCV-2.4.8\opencv\my-build-x64\install

rmdir eigen
mklink /D eigen D:\SDK\eigen-eigen-6b38706d90a9

rmdir dlib
mklink /D dlib dlib-18.7

rmdir poco
mklink /D poco D:\poco\my-build-x64\install

rmdir vlfeat
mklink /D vlfeat D:\SDK\vlfeat-master

rmdir python
mklink /D python C:\Users\alin.draghia\AppData\Local\Enthought\Canopy\App\appdata\canopy-1.3.0.1715.win-x86_64