In order to launch the test class you need to follow these steps:
	-First of all compile the project with CMAKE (or whatever compiler you're using) in your build directory, then build the .exe with make.
	-Then go back to this directory and use qmake --project.
	-qmake
	-make

You will probably have to edit the .pro file before the qmake in order to specify the path to the qwt library in the include path (e.g /mydirectory/qwt6.0.1/src), and to specify the .a used by the test class to run the tests by adding the line "LIBS += -L../build/ImageINSA/lib llibimageinsa" to the .pro file.
