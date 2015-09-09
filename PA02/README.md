Implementation of glut Menus and rotation/spin options for cube
========================================
This is the implementation of a rotating cube that rotates along its y axis.
Currently I am compiling on Mac OSX Yosemite but the makefile is altered to compile on linux.
The program compiles and runs on nomachine linux.  However the special script /usr/NX/scripts/vgl/vglrun ./Matrix must be used to run the program properly.

Menu Functionality includes
Start Spin - Cube starts spinning 
Stop Spin - Cube stops spinning
Exit - Program stops and exits

Clicking the mouse or pressing the A/a key causes the cube to rotate around a fix position clockwise or counterclockwise.

Problems for this program included researching the glut api to determine how to create windows properly, and the logic behind stopping and starting the spinning of the cube.


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
