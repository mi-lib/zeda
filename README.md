ZEDA - Elementary Data and Algorithms
=================================================================
Copyright (C) Tomomichi Sugihara (Zhidao) since 1998

-----------------------------------------------------------------
## [What is this?]

ZEDA is a collection of frequently used data structures and
algorithms including:

- bit operations
- array operations
- list operations
- round-robin table operations
- string operations
- command line option manipulations
- generalized I/O stream
- random number generator
- ZTK format processor

-----------------------------------------------------------------
## [Installation / Uninstallation]

### install

Move to a directly under which you want to install ZEDA, and run:

   ```
   % git clone https://github.com/zhidao/zeda.git
   % cd zeda
   ```

Edit **PREFIX** in *config* file if necessary in order to specify
a directory where the header files, the library and some utilities
are installed. (default: ~/usr)

   - header files: $PREFIX/include/zeda
   - library file: $PREFIX/lib
   - utilities: $PREFIX/bin

Then, make and install.

   ```
   % make && make install
   ```

### uninstall

Do:

   ```
   % make uninstall
   ```

which removes $PREFIX/lib/libzeda.so and $PREFIX/include/zeda.

-----------------------------------------------------------------
## [How to use]

Set your **PATH** and **LD\_LIBRARY\_PATH** environment variables.
This is done by:

   ```
   % export PATH=$PATH:$PREFIX/bin
   % export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib
   ```

for Bourne shell family (bash, zsh, etc.), or by:

   ```
   % set path = ( $path $PREFIX/bin )
   % setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:$PREFIX/lib
   ```

for C shell family (csh, tcsh, etc.).

When you want to compile your code *test.c*, for example, the following line will work.

   ```
   % gcc `zeda-config -L` `zeda-config -I` test.c `zeda-config -l`
   ```

-----------------------------------------------------------------
## [Documents]

Download *doxygen* from http://www.doxygen.org/
and run:

   ```
   % make document
   ```
   
to generate HTML documents under doc/.

-----------------------------------------------------------------
## [Contact]

zhidao@ieee.org
