# EIImage compilation process

This file contains useful information to help you in EIImage compilation.

## Introduction

EIImage is an open source program under the GPL v3 license. Please read the file 'GPL' in this repository for more information.

This software can be compiled under GNU/Linux and Windows, and should be under Mac OS (but this has not been tested yet).

## Tools
Before starting the compilation, please verify that you have all needed tools and the correct versions:

- CMake version 2.8.11 minimum
- A C++ complier

## Source code
If you just cloned this repository, please ensure the detiq-t library submodule is configured and up-to-date, with the commands:
'''
git submodule update --init
'''

You will need to run this command (without `--init`) each time you want to update the submodules to the last revision

## Libraries
EIImage is compiled against some dependencies, you have to check all these dependencies have been installed on your environment.

### Qt 5
#### Windows
You should install Qt5 from the opensource package available on Qt website (http://www.qt.io/download-open-source/ or http://qt-project.org/downloads)

#### GNU/Linux
You may use the developer packages from your distribution. Module needed are Core, Widgets, Gui and LinguistTools. If you have problems, installing Qt5 from the officiel package can be an alternative. See Windows links above.

### RandomLib
Download the RandomLib C++ library from the official website: http://randomlib.sourceforge.net/. Generates compilation environment with CMake and build the library. Default options works well.

RandomLib 1.7, 1.8 and 1.9 are known to work. More recent versions should also work.

### Qwt
Download Qwt 6.0 or newer from the official website: http://sourceforge.net/projects/qwt/
The library must be compiled against Qt, please ensure you use the same version than the one you will use to compile EIImage. The most easier way to comile Qwt is to load the project file (qwt.pro) in Qt Creator, configure the project, run the compilation.

Compilation and installation are documented on the official website: http://qwt.sourceforge.net/qwtinstall.html. If you don't want to install the library on your system after compilation, don't forget to manually locate the folder where the library has been built (thanks to CMAKE_PREFIX_PATH variable) and the include directory (thanks to QWT_INCLUDE_DIR variable). The right value for include dir variable is the 'src' folder in the Qwt main sources folder.

Note: GNU/Linux users can use the package their manager, but this can be problematic if the library binary has been compiled with an incompatble verion of Qt. In that specific case, the library have to be compiled manually.

### Zlib, Jpeg, Png
#### Windows
You can download these libraries from http://gnuwin32.sourceforge.net/. Each library is available as a 'Developer package'. Download the zip and uncompress it on your disk.
Downloads:
- http://gnuwin32.sourceforge.net/packages/zlib.htm
- http://gnuwin32.sourceforge.net/packages/jpeg.htm
- http://gnuwin32.sourceforge.net/packages/libpng.htm

#### GNU/Linux
Developer packages from your distribution may be available. This is the easier way to install these dependencies. CMake will probably detect the paths automatically if you install from your package manager.

## CMake Generation
This is the most important part. Once the generation is done, the compilation shouldn't cause any trouble.

CMake is used here to generate a project file for a specific IDE, or a set of Makefiles compatible with GNU Make or Microsoft NMake.

In general, it is a good idea to generate build environment in an empty folder separated from the source folder.

### Windows users
Depending on the compiler you want to use, you may need to run cmake or cmake-gui from a command line terminal with compilation environment variables pre-defined. For example, to help CMake finding MSVC compiler, you may have to run cmake in a 'Visual Studio Command Prompt'. This can be found in the Start menu. Compiling with MinGW can be achieved by running cmake from 'MinGW shell'.

Since there is no standard path to install libraries, you can choose where you want to install them. If needed, set the CMAKE_PREFIX_PATH variable with the path of dependencies (separated with ';')

### GNU/Linux users
It should be easier to configure compilation for Linux users, since most of the tools are already available from the $PATH environment variable.
If you had to compile manually a dependency, don't forget to run `make install` after the compilation. This by default install libraries and headers under the `/usr/local` prefix, which is generally already in the $PATH

## Compilation
If the previous steps have been done correctly, compilation shouldn't cause any trouble.
