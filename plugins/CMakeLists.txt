
include_directories(../core)

set(SRCS
	plugin.cpp
	Segmentation/segmentation.cpp
)

# add_library(segmentation SHARED Segmentation/segmentation.cpp)

add_library(segmentation SHARED SegmentationTest/segmentation.cpp)

add_library(entropy SHARED Entropy/Entropy.cpp)



