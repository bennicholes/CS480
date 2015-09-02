A simple example of matrix use in OpenGL
========================================
This is the implementation of a rotating cube that rotates along its y axis.
Currently I am compiling on Mac OSX Yosemite so the makefile must be altered to compile on linux.
The current iteration compiles and runs fine on Mac but seg faults in ubuntu. This will be investigated.
At times I have also witnessed instances of ./Matrix run in a row resulted in shaders not being loaded, this is remedied by a make clean and make.


Building This Example
---------------------

*This example requires GLM*
*On ubuntu it can be installed with this command*

>$ sudo apt-get install libglm-dev

*On a Mac you can install GLM with this command(using homebrew)*
>$ brew install glm

To build this example just 

>$ cd build
>$ make

*If you are using a Mac you will need to edit the makefile in the build directory*

The excutable will be put in bin

Additional Notes For OSX Users
------------------------------

Ensure that the latest version of the Developer Tools is installed.
