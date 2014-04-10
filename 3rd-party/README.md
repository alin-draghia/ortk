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

**Linux**: **ln** [OPTION]... TARGET [LINK_NAME] 
