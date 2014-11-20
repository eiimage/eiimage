project(EIImage)
cmake_minimum_required(VERSION 2.8.11)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

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

# Find RandomLib, which must be built and installed before running this compilation
find_package(RandomLib 1.7 REQUIRED)
if(RandomLib_FOUND)
	include_directories(${RandomLib_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Unable to find Random library. Please clone, compile and install it before compiling eiimage.")
endif()

# Add CMake modules path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Detiq-T is a submodule, it is built automatically before building eiimage
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/detiq-t AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/detiq-t)
	add_subdirectory(lib/detiq-t)
	include_directories(lib/detiq-t/ImageIn lib/detiq-t/GenericInterface)
else()
	message(FATAL_ERROR "The Detiq-T project should be cloned before executing this compilation. "
		"Please use 'git submodule init' and 'git submodule update' to automatically get the source "
		"for Detiq-T (ImageIn & GenericInterface libraries), and re-run CMake.")
endif()

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
find_package(Qwt 6.0)
include_directories(${QWT_INCLUDE_DIRS})

# We want to generate everything in the right target folder, for easier deployment
set(DEPLOY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/eiimage CACHE PATH "The folder to use for deployment." FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DEPLOY_OUTPUT_DIRECTORY})

add_subdirectory(core)
include_directories(core)

add_subdirectory(plugins)
add_subdirectory(app)

if(WIN32)
	get_target_property(Qt5Core Qt5::Core LOCATION)
	file(COPY ${Qt5Core} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Gui Qt5::Gui LOCATION)
	file(COPY ${Qt5Gui} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Widgets Qt5::Widgets LOCATION)
	file(COPY ${Qt5Widgets} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Xml Qt5::Xml LOCATION)
	file(COPY ${Qt5Xml} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Script Qt5::Script LOCATION)
	file(COPY ${Qt5Script} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Print Qt5::PrintSupport LOCATION)
	file(COPY ${Qt5Print} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Opengl Qt5::OpenGL LOCATION)
	file(COPY ${Qt5Opengl} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	get_target_property(Qt5Svg Qt5::Svg LOCATION)
	file(COPY ${Qt5Svg} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})

	get_filename_component(QT_DIR ${Qt5Core} DIRECTORY)
	# Copy icuuc, icudt & icuin**.dll files
	file(GLOB ICUS "${QT_DIR}/icu*.dll")
	foreach(__FILE ${ICUS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()

	# Copy MinGW dependencies
	file(GLOB GCCS "${QT_DIR}/*gcc_s*.dll")
	foreach(__FILE ${GCCS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()
	file(GLOB GCCS "${QT_DIR}/*stdc++*.dll")
	foreach(__FILE ${GCCS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()
	file(GLOB THREADS "${QT_DIR}/*winpthread*.dll")
	foreach(__FILE ${THREADS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()

	get_target_property(RandomLocation RandomLib IMPORTED_LOCATION_RELEASE)
	file(COPY ${RandomLocation} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})

	get_filename_component(QWT_LIB_DIR ${QWT_LIBRARY} DIRECTORY)
	file(GLOB QWT_SHARED_LIBS "${QWT_LIB_DIR}/*.dll")
	foreach(__FILE ${QWT_SHARED_LIBS})
		file(COPY ${__FILE} DESTINATION ${DEPLOY_OUTPUT_DIRECTORY})
	endforeach()

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
endif()
