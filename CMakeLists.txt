project(ImageINSA)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_BUILD_TYPE "Release")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#Modif Pazat : repertoire contenant les fichiers de configuration .cmake pour QT5. !!! Modifier le chemin pour qu'il dépende des variables d'environnement !!!
#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.6.1\\5.6\\msvc2015_64\\lib\\cmake\\")

#Modif Pazat : repertoire contenant les fichiers de configuration .cmake pour QT5.
set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.6.1\\5.6\\mingw49_32\\lib\\cmake")

# Find the Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Script REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Svg)
find_package(Qt5OpenGL)
find_package(Qt5PrintSupport)

# We want to generate everything in the right target folder, for easier deployment
file(MAKE_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY}/lib)
set(DEPLOY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ImageINSA CACHE PATH "The folder to use for deployment." FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY})

set (CMAKE_PREFIX_PATH "C:\\RandomLib-mingw32")

# Find RandomLib, which must be built and installed before running this compilation
find_package(RandomLib 1.7 REQUIRED)
if(RandomLib_FOUND)
	include_directories(${RandomLib_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Unable to find Random library. Please clone, compile and install it before compiling ImageINSA.")
endif()

# Add CMake modules path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Detiq-T is a submodule, it is built automatically before building ImageINSA
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/detiq-t AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/detiq-t)
	add_subdirectory(lib/detiq-t)
	include_directories(lib/detiq-t/ImageIn lib/detiq-t/GenericInterface)
else()
	message(FATAL_ERROR "The Detiq-T project should be cloned before executing this compilation. "
		"Please use 'git submodule init' and 'git submodule update' to automatically get the source "
		"for Detiq-T (ImageIn & GenericInterface libraries), and re-run CMake.")
endif()

set (CMAKE_MODULE_PATH "C:\\Program Files\\CMake\\share\\cmake-3.6\\Modules\\")
set (CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\GnuWin32\\")


find_package(JPEG)
if(JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})
else()
	message(FATAL_ERROR "Cannot find JPEG library. Please add the JPEG library directory to CMAKE_PREFIX_PATH.")
endif()

find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Cannot find PNG library. Please add the PNG library directory to CMAKE_PREFIX_PATH.")
endif()

find_package(Threads REQUIRED)

# Simple inclusion, more control have already been performed
# in previous Detiq-T lookup (GenericInterface)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(CMAKE_PREFIX_PATH "C:\\Qwt-6.1.2\\")
set(QWT_LIBRARY "C:\\Qwt-6.1.2\\lib\\qwt.dll")
set(QWT_INCLUDE_DIR "C:\\Qwt-6.1.2\\include")

find_package(Qwt 6.0 REQUIRED)
	include_directories(${QWT_INCLUDE_DIRS})


add_subdirectory(core)
include_directories(core)

add_subdirectory(plugins)
add_subdirectory(app)

# Deploy all needed dynamic libraries
if(WIN32)
	# libRandom.dll
	get_target_property(RandomLocation RandomLib IMPORTED_LOCATION_RELEASE)
	file(COPY ${RandomLocation} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})

	# Qwt.dll
	get_filename_component(QWT_LIB_DIR ${QWT_LIBRARY} DIRECTORY)
	file(GLOB QWT_SHARED_LIBS "${QWT_LIB_DIR}/*.dll")
	foreach(__FILE ${QWT_SHARED_LIBS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()

	# Jpeg and png dlls
	get_filename_component(JPEG_LIB_DIR ${JPEG_LIBRARY} DIRECTORY)
	file(GLOB JPEG_SHARED_LIBS "${JPEG_LIB_DIR}/../bin/*.dll")
	foreach(__FILE ${JPEG_SHARED_LIBS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()
	get_filename_component(PNG_LIB_DIR ${PNG_LIBRARY} DIRECTORY)
	file(GLOB PNG_SHARED_LIBS "${PNG_LIB_DIR}/../bin/*.dll")
	foreach(__FILE ${PNG_SHARED_LIBS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()

	# Qt and compiler dll
	if(Qt5Core_VERSION_STRING VERSION_GREATER 5.3)
		if(CMAKE_BUILD_TYPE MATCHES Release)
			set(DEPLOY_FLAGS "--release")
		else()
			set(DEPLOY_FLAGS "--debug")
		endif()
	endif()
	add_custom_target(WinQtDeploy ALL
		windeployqt ${DEPLOY_OUTPUT_DIRECTORY}/qwt.dll
		COMMAND windeployqt ${DEPLOY_FLAGS} ${DEPLOY_OUTPUT_DIRECTORY}/imageinsa.exe
		COMMENT "Copy Qt & compiler DLL to the target directory")
	add_dependencies(WinQtDeploy imageinsa)
endif()
