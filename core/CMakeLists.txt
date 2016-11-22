
find_package(Qt5Widgets REQUIRED)

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
