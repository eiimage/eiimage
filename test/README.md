## - COMPILATION 
- In order to launch the test class you need to follow these steps :
	- First of all compile the project with CMAKE (or whatever compiler you're using) in your build directory, then build the .exe with make.
	- Then go back to this directory and use qmake --project.
	- qmake
	- make

- You will probably have to edit the .pro file before the qmake in order to specify the path to the qwt library in the include path (e.g /mydirectory/qwt6.0.1/src), and to specify the .a used by the test class to run the tests by adding the line "LIBS += -L../build/ImageINSA/lib llibimageinsa" to the .pro file.

- The test directory is composed of three sub directories named ref, res and src. Res should be empty at the end of your compilation, because it contains the resulting image after application of the operations. Src contains all the source which the test are applied to. Ref contains the images reference with each operation applied on through another software (here matlab).

- Everytime you execute the test you have to run a make distclean, then do the qmake, make and LD_LIBRARY_PATH=/home/bprovot/Documents/eiimage/build/ImageINSA/lib  ./test, in order to empty the src directory and not run test on a previous value of the resulting images.

## - GENERATING REFERENCE IMAGES

- We used MATLAB for references. To do so we had to use matlab function trying to mimic imageINSA functions, that can be found in the MATLAB folder.

- To open a .vff image in MATLAB there is a function called vff.m findable in the so called MATLAB folder. It needs the path to the image, if you give no parameter it will open a window to select the image.