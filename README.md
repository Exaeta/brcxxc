# Binary Resources for C++ Compiler (BRCXXC)

Version 2.1.3

The Binary Resources for C++ Compiler is a simple cross platform compiler that
converts binary files to C++ header and source files for inclusion in other 
projects.

I found other solutions to this problem deficient. xxd doesn't allow you to
set the name of the variables to anything other than the files name. objdump,
ld, both have the same issues in general.


Version 2.x.x has been renamed BRCXXC to better reflect that it is intended for 
use with C++. In addition, there has been a major change to how the compiler is
run and executed.

The manual for how to use brcxxc is available at: https://rpnx.net/brcxxc-2.1.html
