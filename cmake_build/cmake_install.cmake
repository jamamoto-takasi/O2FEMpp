# Install script for directory: /home/hbui/workspace2/oofem-sd_cpp

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
  IF(EXISTS "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so"
         RPATH "")
  ENDIF()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so")
FILE(INSTALL DESTINATION "/home/hbui/workspace2/oofem-sd_cpp/libs" TYPE SHARED_LIBRARY FILES "/home/hbui/workspace2/oofem-sd_cpp/cmake_build/OOFEM.so")
  IF(EXISTS "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so")
    FILE(RPATH_REMOVE
         FILE "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/OOFEM.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so"
         RPATH "")
  ENDIF()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so")
FILE(INSTALL DESTINATION "/home/hbui/workspace2/oofem-sd_cpp/libs" TYPE SHARED_LIBRARY FILES "/home/hbui/workspace2/oofem-sd_cpp/cmake_build/libFoxGL.so")
  IF(EXISTS "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/hbui/workspace2/oofem-sd_cpp/libs/libFoxGL.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/hbui/workspace2/oofem-sd_cpp/cmake_build/solver/SuperLU_4.3/cmake_install.cmake")
  INCLUDE("/home/hbui/workspace2/oofem-sd_cpp/cmake_build/tests/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/hbui/workspace2/oofem-sd_cpp/cmake_build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/hbui/workspace2/oofem-sd_cpp/cmake_build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
