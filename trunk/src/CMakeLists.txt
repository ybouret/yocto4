########################################################################
## Top level CMakeLists.txt for all projects
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(yocto4)

########################################################################
# Main rules and helpes
########################################################################
INCLUDE(share/rules.cmake)

SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../sdk)

########################################################################
# mandatory project
########################################################################
ADD_SUBDIRECTORY(y-main)

########################################################################
# Read projects to exclude
########################################################################
STRING( REGEX REPLACE "[ \t]+" ";" YOCTO_EXCLUDE "$ENV{YOCTO_EXCLUDE}")
MESSAGE( STATUS "YOCTO_EXCLUDE=${YOCTO_EXCLUDE}")

MACRO(YOCTO_IGNORE dir)
  SET(no_${dir} ON)
ENDMACRO(YOCTO_IGNORE)

FOREACH(prj ${YOCTO_EXCLUDE})
  YOCTO_IGNORE(${prj})
ENDFOREACH()

MACRO(YOCTO_BUILD dir)
  IF( no_${dir} )
    MESSAGE( STATUS "NOT BUILDING y-${dir}" )
  ELSE()
    ADD_SUBDIRECTORY(y-${dir})
  ENDIF()
ENDMACRO(YOCTO_BUILD)

########################################################################
# Record projects
########################################################################
YOCTO_BUILD(thr)
YOCTO_BUILD(rx)
YOCTO_BUILD(fs)
YOCTO_BUILD(kr)
YOCTO_BUILD(net)
YOCTO_BUILD(mpk)
YOCTO_BUILD(mk)
YOCTO_BUILD(random)
YOCTO_BUILD(fft)
YOCTO_BUILD(spade)
YOCTO_BUILD(lua)
YOCTO_BUILD(lua++)
YOCTO_BUILD(z)
YOCTO_BUILD(z++)
YOCTO_BUILD(mpi)
YOCTO_BUILD(ocl)

INSTALL( FILES share/rules.cmake share/fltk.cmake share/opencl.cmake DESTINATION share/yocto )

########################################################################
IF(UNIX)
  GET_FILENAME_COMPONENT(CXX_NICKNAME ${CMAKE_CXX_COMPILER} NAME)
ELSE()
  GET_FILENAME_COMPONENT(CXX_NICKNAME ${CMAKE_CXX_COMPILER} NAME_WE)
ENDIF()

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "yocto")
SET(CPACK_PACKAGE_VENDOR "Yann Bouret")
SET(CPACK_PACKAGE_VERSION "4.0" )

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER    "Yann Bouret")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY  "C++ cross platform libraries" )
SET(CPACK_PACKAGE_DESCRIPTION          "Yocto Offerts C++ Tools ans Objects" )
SET(CPACK_DEBIAN_PACKAGE_SECTION       "Development" )
EXEC_PROGRAM( dpkg ARGS "--print-architecture" OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE )

IF( ON_MACOSX OR WIN32 )
  SET(CPACK_PACKAGE_FILE_NAME "yocto-${CMAKE_SYSTEM_NAME}-${CXX_NICKNAME}")
ELSE()
  SET(CPACK_PACKAGE_FILE_NAME "yocto-${CMAKE_SYSTEM_NAME}-${BUILD_MACHINE}-${CXX_NICKNAME}")
ENDIF()

SET(CPACK_PACKAGING_INSTALL_PREFIX "/opt/local")
SET(CPACK_GENERATOR "ZIP")

INCLUDE(CPack)

