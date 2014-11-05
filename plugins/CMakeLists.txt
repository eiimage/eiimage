
include_directories(../core)

set(SRCS
	plugin.cpp
	Segmentation/segmentation.cpp
)
add_library(eiimage_plugins ${SRCS})
