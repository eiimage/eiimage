
find_package(Qt5Widgets REQUIRED)

 set(CMAKE_PREFIX_PATH "C:\\Qt\\Qwt-6.1.2\\")
 #set(QWT_LIBRARY "C:\\Qwt-6.1.2\\lib\\qwt.dll")
set(QWT_LIBRARY C:/Qt/Qwt-6.1.2/lib/qwt.dll)
 set(QWT_INCLUDE_DIR "C:\\Qt\\Qwt-6.1.2\\include")

find_package(Qwt 6.0 REQUIRED)
if(QWT_FOUND)
	include_directories(${QWT_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Cannot find Qwt library. Please add the Qwt library directory to CMAKE_PREFIX_PATH.")
endif()

set(SRCS
	BuiltinOpSet.cpp
	BuiltinOpSet.h
	ImgParam.cpp
	ImgParam.h
	Input.h
	IntParam.cpp
	IntParam.h
	Operation.cpp
	Operation.h
	OpSet.h
	Parameter.h
	Plugin.cpp
	Plugin.h
	PlugOperation.cpp
	PlugOperation.h
)
add_library(core SHARED ${SRCS} $<TARGET_OBJECTS:GenericInterface> $<TARGET_OBJECTS:ImageIn>)
target_link_libraries(core
	Qt5::Widgets
	${QWT_LIBRARY}
	${QWT_LIBRARIES}
	${JPEG_LIBRARY}
	${PNG_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)
