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
ADD_SUBDIRECTORY(main)

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
    MESSAGE( STATUS "NOT BUILDING ${dir}" )
  ELSE()
    ADD_SUBDIRECTORY(${dir})
  ENDIF()
ENDMACRO(YOCTO_BUILD)

########################################################################
# Record projects
########################################################################
MESSAGE( STATUS "" )
MESSAGE( STATUS "---------------- <modules>  ----------------" )

YOCTO_BUILD(lingua)
YOCTO_BUILD(kr)
YOCTO_BUILD(net)
YOCTO_BUILD(mpa)
YOCTO_BUILD(mk)
YOCTO_BUILD(spade)
YOCTO_BUILD(gems)
YOCTO_BUILD(lua++)
YOCTO_BUILD(z++)
YOCTO_BUILD(bz2++)
YOCTO_BUILD(mpi)
YOCTO_BUILD(ocl)
YOCTO_BUILD(gfx)
YOCTO_BUILD(json)
YOCTO_BUILD(chemical)
YOCTO_BUILD(visit)
YOCTO_BUILD(fltk)
YOCTO_BUILD(fovea)
YOCTO_BUILD(python)
YOCTO_BUILD(R)

MESSAGE( STATUS "---------------- </modules> ----------------" )
MESSAGE( STATUS "" )

########################################################################
# Record third_party
########################################################################
MESSAGE( STATUS "" )
MESSAGE( STATUS "---------------- <third_party>  ----------------" )
ADD_SUBDIRECTORY(third_party)
MESSAGE( STATUS "---------------- </third_party>  ----------------" )
MESSAGE( STATUS "" )


INSTALL( FILES 
	share/create.sh 
	share/rules.cmake 
	share/fltk.cmake 
	share/opencl.cmake 
	share/openmp.cmake
	share/swig.cmake 
	share/visit.cmake
	DESTINATION share/yocto )

########################################################################
MESSAGE( STATUS "" )
MESSAGE( STATUS "---------------- <packaging>  ----------------" )

IF(UNIX)
  GET_FILENAME_COMPONENT(CXX_NICKNAME ${CMAKE_CXX_COMPILER} NAME)
ELSE()
  GET_FILENAME_COMPONENT(CXX_NICKNAME ${CMAKE_CXX_COMPILER} NAME_WE)
ENDIF()

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "yocto")
SET(CPACK_PACKAGE_VENDOR "Yann Bouret")

FIND_PROGRAM(SVNVERSION svnversion)
MESSAGE( STATUS "SVNVERSION=${SVNVERSION}" )

IF(${SVNVERSION} STREQUAL "SVNVERSION-NOTFOUND")
  SET(CPACK_PACKAGE_VERSION "4.0" )
ELSE()
  EXEC_PROGRAM( ${SVNVERSION} ARGS ${PROJECT_SOURCE_DIR}/.. OUTPUT_VARIABLE THE_REVISION)
  SET(CPACK_PACKAGE_VERSION "4.r${THE_REVISION}" )
ENDIF()
MESSAGE( STATUS "CPACK_PACKAGE_VERSION=${CPACK_PACKAGE_VERSION}" )

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER    "Yann Bouret")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY  "C++ cross platform libraries" )
SET(CPACK_PACKAGE_DESCRIPTION          "Yocto Offerts C++ Tools ans Objects" )
SET(CPACK_DEBIAN_PACKAGE_SECTION       "Development" )
EXEC_PROGRAM( dpkg ARGS "--print-architecture" OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE )

IF( YOCTO_MACOSX OR WIN32 )
  SET(CPACK_PACKAGE_FILE_NAME "yocto-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CXX_NICKNAME}")
ELSE()
  SET(CPACK_PACKAGE_FILE_NAME "yocto-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${BUILD_MACHINE}-${CXX_NICKNAME}")
ENDIF()

########################################################################
## where to install
########################################################################

SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")


########################################################################
## default generator
########################################################################
IF( YOCTO_MACOSX )
	SET(CPACK_GENERATOR "DragNDrop" )
ELSE()
	SET(CPACK_GENERATOR "ZIP")
ENDIF()

MESSAGE( STATUS "CPACK_GENERATOR='${CPACK_GENERATOR}'" )

INCLUDE(CPack)
MESSAGE( STATUS "---------------- </packaging> ----------------" )
MESSAGE( STATUS "" )

