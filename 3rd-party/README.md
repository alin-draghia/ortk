#3rd Party Directory

Use **symbolic links** for the libs

* /3rd-party
    * /opencv (symlink)
    	* /include
		* /lib
		* /bin
	* /poco (symlink)
		* /include
		* /lib
		* /bin
	* ...


**Windows**: (as admin) **mklink** [[/d] | [/h] | [/j]] <Link\> <Target\>

> **/d** Creates a directory symbolic link. By default, mklink creates a file symbolic link.

> **/h** Creates a hard link instead of a symbolic link.

> **/j** Creates a Directory Junction.

> **<Link\>** Specifies the name of the symbolic link that is being created.

> **<Target\>** Specifies the path (relative or absolute) that the new symbolic link refers to.

> **/?** Displays help at the command prompt.

**Linux**: **ln** [OPTION]... TARGET [LINK_NAME] 

## Building **VLfeat** ##

* Open _Visual Studio Command Prompt_
* nmake /f Makefile.mak MSVSVER=120 ARCH=win64


## Createing the symbolic links ##

* Open Command Prompt as __Administrator__ 
* mklink /D opencv D:\SDK\OpenCV-2.4.8\opencv\my-build-x64\install
* mklink /D eigen D:\SDK\eigen-eigen-6b38706d90a9
* mklink /D dlib D:\SDK\dlib-18.4
* mklink /D poco D:\poco\my-build-x64\install
* mklink /D vlfeat D:\SDK\vlfeat-master
