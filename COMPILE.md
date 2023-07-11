# ImageINSA compilation process

This file contains useful information to help you in ImageINSA compilation.

## Introduction

ImageINSA is an open source program under the GPL v3 license. Please read the file 'GPL' in this repository for more information.

This software can be compiled under GNU/Linux and Windows, and should be under Mac OS (but this has not been tested yet).

## Tools
Before starting the compilation, please verify that you have all needed tools and the correct versions:

- CMake version 2.8.11 minimum  
  *NB: Do not change the default installation directory of CMake*
- A C++ compiler (gcc, mingw)

## Source code
If you just cloned this repository, please ensure the detiq-t library submodule is configured and up-to-date, with the commands:

    git submodule update --init

You will need to run this command (without `--init`) each time you want to update the submodule to the last revision

## Libraries
ImageINSA is compiled against some dependencies, you have to check all these dependencies have been installed on your environment.

Since the Zlib, Jpeg and Png libraries are already compiled in 32-bit, install the 32-bit versions of the dependencies, otherwise there will be linkage problems.

### Qt 5
#### Windows
You should install Qt5 from the opensource package available on Qt website (http://www.qt.io/download-open-source/ or http://qt-project.org/downloads)  

*NB: Do not change the default installation directory*

#### GNU/Linux
You may use the developer packages from your distribution. Module needed are Core, Widgets, Gui and LinguistTools. If you have problems, installing Qt5 from the officiel package can be an alternative. See Windows links above.

### RandomLib
Download the RandomLib C++ library from the official website: http://randomlib.sourceforge.net/. Generates compilation environment with CMake and build the library. Default options works well.

RandomLib 1.7, 1.8 and 1.9 are known to work. Newer versions should also be ok.

**Install RandomLib on Windows:**

Open Qt terminal :

	cd {path}\RandomLib-1.10 //({path} is where you extracted the downloaded RandomLib zip file)  
	mkdir build  
	cd build  
	cmake -G "MinGW Makefiles" ..  
	mingw32-make  
	mingw32-make install

### Qwt
Download Qwt 6.0 or newer from the official website: http://sourceforge.net/projects/qwt/
The library must be compiled against Qt, please ensure you use the same version than the one you will use to compile ImageINSA. The most easier way to comile Qwt is to load the project file (qwt.pro) in Qt Creator, configure the project, run the compilation.

Compilation and installation are documented on the official website: http://qwt.sourceforge.net/qwtinstall.html.

**Install Qwt on Windows:**

Open Qt terminal :

	cd {path}\qwt-6.1.3 //({path} is where you extracted the downloaded qwt zip file)
	qmake qwt.pro
	mingw32-make
	mingw32-make install

Note: 

1. GNU/Linux users can install the package provided by their distribution, if it exists, but this can be problematic if the library binary has been compiled with an incompatible version of Qt. In that specific case, the library have to be compiled manually. 

2. If the OPENGL library is missing (fatal error: GL / gl.h: No such file or directory), the installation of build-essential libgl1-mesa-dev will solve this problem.
3. If you choose to install Qwt directly by make without using qmake, you will need to manually copy some files from Qwt directory to your Qt directory (required on both Linux and Windows systems): 
   1. Copy qwt-6.1.5/designer/plugins/designer/libqwt_designer_plugin.so (or .dll) to Qt/5.15.1/gcc_64 /plugins/designer.
   2. Copy all files in qwt-6.1.5/lib to Qt/5.15.1/gcc_64/lib.
   3. Create a new folder in Qt/5.15.1/gcc_64 /include and name it qwt, copy all the header files under qwt-6.1.5/src to the new created qwt folder.
4. If you fail to compile Qwt 6.2.0 because of SplineTest, you have to replace `int main()` by `int main(int argc, char *argv[])`


### Zlib, Jpeg, Png
#### Windows
You can get these libraries from http://gnuwin32.sourceforge.net/. Download and install the 'Complete package, except sources' from:
- http://gnuwin32.sourceforge.net/packages/zlib.htm
- http://gnuwin32.sourceforge.net/packages/jpeg.htm
- http://gnuwin32.sourceforge.net/packages/libpng.htm

Note: If you don't have the right to install packages on your machine, you can download both 'Binaries' and 'Developer files' zip packages for each library. Then, uncompress it to a folder (example: C:\GnuWin32). This will automatically copy libraries into 'bin', 'lib' and 'include' sub-folders.

#### GNU/Linux
It's easier to install these dependencies on Linux, developer packages from your distribution may be available, try zlib1g, libjpeg-dev and libpng-dev for example. CMake will probably detect dependencies more easily if you install them with the package manager.

## CMake Generation
This is the most important part. Once the generation is done, the compilation shouldn't cause any trouble.

CMake is used here to generate a project file for a specific IDE, or a set of Makefiles compatible with GNU Make or Microsoft NMake.

In general, it is a good idea to generate build environment in an empty folder separated from the source folder.

### Windows users
Depending on the compiler you want to use, you may need to run `cmake` or `cmake-gui` from a command line terminal with compilation environment variables pre-defined. For example, to help CMake finding MSVC compiler, you may have to run cmake in a 'Visual Studio Command Prompt'. This can be found in the Start menu. Compiling with MinGW can be achieved by running cmake from 'MinGW shell'.

The CMake scripts shipped with ImageINSA try to locate automatically all libraries and tools needed to compile it. But in some case, this is impossible because there is no common standard path were all libraries should be installed. To help CMake find needed libraries, you can set some variables when running cmake:

- CMAKE_PREFIX_PATH will be used by CMake to serach for libraries needed. You can put local paths were some dependencies are stored. Use the separator of your OS (';' for Windows). Example: `-DCMAKE_PREFIX_PATH="C:/Program Files/GnuWin32";D:/libs/RandomLib-1.10-build`
- \<libname\>_DIR can be used to help CMake finding a specific library
- \<libname\>_INCLUDE_DIR can be used to help Cmake finding headers for a specific library

**Example of CMake generation procedure on Windows :**

Open Qt terminal :

	cd {path}\eiimage //(where you cloned eiimage repository)
	mkdir build
	cd build
	cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Program Files (x86)/GnuWin32";C:\Qwt-6.1.3 ..
If cmake command does not work, run `set PATH=%PATH%;C:\Qt\5.9.1\mingw53_32\bin` before it.

### GNU/Linux users
It should be easier to configure compilation for Linux users, since most of the tools are already available from the $PATH environment variable.
If you had to compile manually a dependency, don't forget to run `make install` after the compilation. By default, the command installs libraries and headers under the `/usr/local` prefix, which is usually already in the $PATH.

## Compilation
If the previous steps have been done correctly, compilation shouldn't cause any trouble.

### Windows users

Open Qt terminal :

	cd {path}\eiimage //({path} is where you cloned eiimage repository)
	cd build
	mingw32-make -j4