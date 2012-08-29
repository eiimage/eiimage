# $Id: 0ff45cb4371024d7d85cc9884c6372c568921c65 $

# Install the header files including the generated Config.h (which is in
# the build tree).
file (GLOB HEADERS *.hpp)
install (FILES ${HEADERS} DESTINATION include/RandomLib)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/Config.h
  DESTINATION include/RandomLib)
