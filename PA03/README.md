Implementation of glut Menus and rotation/spin options for cube
========================================
This is the implementation of a rotating cube with a moon that orbits around it.
Currently I am compiling on Mac OSX Yosemite but the makefile is altered to compile on linux.
The program compiles and runs on nomachine linux.  However the special script /usr/NX/scripts/vgl/vglrun ./Matrix must be used to run the program properly.

*New Functionality with PAO3*
Moon included and rendered that will continously orbit the planet no matter its path.  
I chose to not make the moon spin on its y axis, however I did include it in the code on line 223.  If line 223 is uncommented the moon will spin when the Start Spin option is selected on the right-click menu.
Keyboard controls were also included. The right arrow key will cause the planet to make its orbit counter clockwise. The left arrow key will cause the planet to make its orbit clockwise.
The seg fault problem in PA02 with the menu is solved as far as I can tell with the addition of the freeglut library. I also made a change through an exit flag that is checked for at the begining of update using glutFreeMainLoop() instead of using a exit(0) inside of the menu select.


Menu Functionality includes
Start Spin - Cube starts spinning 
Stop Spin - Cube stops spinning
Exit - Program stops and exits

Clicking the mouse or pressing the A/a key causes the cube to rotate around a fix position clockwise or counterclockwise.

Problems for this program included deciding how the moon would be able to keep orbit around the planet.  Another problem I ran into was when I implemented the moon orbiting and spinning, but I forgot to initialize and render the moon.


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
