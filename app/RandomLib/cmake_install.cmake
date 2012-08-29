# Install script for directory: /home/sacha/Downloads/RandomLib-1.5/include/RandomLib

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/RandomLib" TYPE FILE FILES
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRNormalK.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/ExponentialDistribution.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomType.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomPower2.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/Random.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/LeadingZeros.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/InversePiProb.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/InverseEProb.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRNormal.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomCanonical.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomMixer.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRNormalR.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRExponential.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/ExactPower.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRUniform.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRExponentialL.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/NormalDistribution.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomSelect.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomNumber.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/ExactExponential.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomSeed.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomAlgorithm.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/ExponentialProb.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/RandomEngine.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/MPFRRandom.hpp"
    "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/ExactNormal.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/RandomLib" TYPE FILE FILES "/home/sacha/Downloads/RandomLib-1.5/include/RandomLib/Config.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

