project(EIImage)
cmake_minimum_required(VERSION 2.8.11)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Add CMake modules path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Detiq-T is a submodule, it is built automatically before building eiimage
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/detiq-t AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/detiq-t)
	add_subdirectory(lib/detiq-t)
     include_directories(lib/detiq-t/ImageIn lib/detiq-t/GenericInterface)
else()
	message(FATAL_ERROR "The Detiq-T project should be cloned before excuting this compilation. "
		"Please use 'git submodule init' and 'git submodule update' to automatically get the source "
		"for Detiq-T, and re-run CMake.")
endif()

add_subdirectory(core)
include_directories(core)

# Too much compile errors, have to be fixed:
# add_subdirectory(plugins)
add_subdirectory(app)

