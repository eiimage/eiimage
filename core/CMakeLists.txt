
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
add_library(core ${SRCS})
target_link_libraries(core
	GenericInterface
	ImageIn
	Qt5::Widgets
)
