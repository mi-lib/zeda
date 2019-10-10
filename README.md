ZEDA - Elementary Data and Algorithms
Copyright (C) 1998 Tomomichi Sugihara (Zhidao)

# What is this?

ZEDA is a collection of frequently used data structures and
algorithms including:  
- bit operations
- array operation
- list operation
- round-robin table
- string operations
- command line option manipulation
- generalized I/O stream
- random number generator

# Installation / Uninstallation
### install
1. Clone this repository where you want.  
	```
	$ git clone https://github.com/zhidao/zeda.git
	```  
  
1. Enter the directory.  
	```
	$ cd zeda
	```  
  
1. Edit config file if necessary.  
	* PREFIX  
		directory where the library is installed.
    `~/usr` as a default. In this case, header files
    and library are installed under `~/usr/include`
    and `~/usr/lib`, respectively.  
  
1. Make it.  
	```
	$ make
	```  
  
1. Install it.  
	```
	$ make install
	```  
	Or,
	```
	$ cp -a lib/libzeda.so $PREFIX/lib/`
	$ cp -a include/zeda $PREFIX/include/`
	$ cp -a bin/* $PREFIX/bin/
	```

### uninstall
1. Uninstall it.  
	```
	$ make uninstall
	```  
	Or,  
	delete `$PREFIX/lib/libzeda.so` and `$PREFIX/include/zeda`.

# How to use

You may need to set your PATH and LD_LIBRARY_PATH environment variables.  
This is done by:
```
$ export PATH=$PATH:$PREFIX/bin
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib
```  
if your working shell is Bourne shell (bash, zsh, etc.),  
or by:
```
$ set path = ( $path $PREFIX/bin )
$ setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:$PREFIX/lib
```  
if your working shell is C shell (csh, tcsh, etc.).

When you want to compile your code test.c, for example, the following line will work.  
```
$ gcc `zeda-config -L` `zeda-config -I` test.c `zeda-config -l`
```

# Documents

If you have doxygen:  
http://www.doxygen.org/  
the following works:  
```
$ make document
```  
to generate HTML documents under doc/

# Contact

zhidao@ieee.org
