Implementation of glut Menus and rotation/spin options for cube
========================================
This is the implementation of a rotating cube with a moon that orbits around it.
Currently I am compiling on Mac OSX Yosemite but the makefile is altered to compile on linux.
The program compiles and runs on nomachine linux.  However the special script /usr/NX/scripts/vgl/vglrun ./Matrix must be used to run the program properly.


Add a file through the command line.  For example run ./Matrix table.obj to load the object I created in blender.
This is an implementation of a .obj model loader with opengl.  The model loader will load a file through a command line argument and produce output onto the screen.  The model is colored through the geometry.color array.

For this project I removed the cube, moon, and spinning functionality.
Included in the bin folder.

Problems for this program included how I wanted to pass around the vertices, as well as how to load from the file and what to look for in the file. This was mostly explained through the provided tutorial. On my computer the table doesn't look quite right but the dragon model does.


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
