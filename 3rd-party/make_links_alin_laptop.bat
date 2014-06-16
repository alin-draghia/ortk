@echo off

set my_dir=%~dp0%
cd /D %my_dir%


rmdir opencv
mklink /D opencv d:\dev\opencv-2.4.8\opencv\my-build-x64\install

rmdir eigen
mklink /D eigen d:\dev\eigen\eigen-eigen-6b38706d90a9

rmdir dlib
mklink /D dlib d:\dev\dlib-18.6\dlib-18.6

rmdir poco
mklink /D poco d:\dev\poco-develop\my_build\install

rmdir vlfeat
mklink /D vlfeat d:\dev\vlfeat-master

rmdir python
mklink /D python d:\dev\Anaconda