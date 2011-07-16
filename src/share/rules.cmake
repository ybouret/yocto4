########################################################################
##
########################################################################

SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

########################################################################
##
########################################################################
EXEC_PROGRAM( uname ARGS "-m" OUTPUT_VARIABLE BUILD_MACHINE )
#MESSAGE( STATUS "BUILD_MACHINE=${BUILD_MACHINE}" )

SET(ON_MACOSX OFF)
IF( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  SET(ON_MACOSX ON)
ENDIF()

########################################################################
## tuning compilers flags
########################################################################
GET_FILENAME_COMPONENT(CC_NAME ${CMAKE_C_COMPILER} NAME )


########################################################################
## GNU
########################################################################
IF( "${CC_NAME}" MATCHES "gcc.*" )
  MESSAGE( STATUS "Using GNU compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions -Weffc++ ${MY_CXXFLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1")

ENDIF()

########################################################################
## clang
########################################################################
IF( "${CC_NAME}" MATCHES "clang.*" )
  MESSAGE( STATUS "Using CLANG compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions -Weffc++ ${MY_CXXFLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1")
ENDIF()



########################################################################
## clang
########################################################################
IF( "${CC_NAME}" MATCHES "icc.*" )
  MESSAGE( STATUS "Using Intel compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions${MY_CXXFLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1")
ENDIF()

########################################################################
## PathScale
########################################################################
IF( "${CC_NAME}" MATCHES "path.*" )
  MESSAGE( STATUS "Using PathScale compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions${MY_CXXFLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1")
ENDIF()


########################################################################
## miscrosoft cl
########################################################################
IF( "${CC_NAME}" STREQUAL "cl.exe" )
  MESSAGE( STATUS "Using Microsoft cl.exe" )
  
  SET(COMMON_C_FLAGS      "${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS}" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -Ox -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "${MY_CXXFLAGS} -EHsc" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS}" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -Ob2it -DNDEBUG=1")
  
ENDIF()


########################################################################
# Common Flags
# Adjusting Linker Flags
########################################################################
MACRO(TARGET_LINK_YOCTO tgt)
	MESSAGE( STATUS "${tgt} will use yocto libraries" )

	####################################################################
	## Reverse order extra libraries
	####################################################################
	FOREACH( extra ${ARGN} )
		TARGET_LINK_LIBRARIES( ${tgt} y-${extra} )
	ENDFOREACH()

	####################################################################
	## Yocto Core C/C++
	####################################################################
	TARGET_LINK_LIBRARIES( ${tgt} yocto )

	####################################################################
	## Platform Specific Flags
	####################################################################
	IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
		TARGET_LINK_LIBRARIES( ${tgt} pthread )
	ENDIF()
	IF( ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" )
		TARGET_LINK_LIBRARIES( ${tgt} pthread )
	ENDIF()

	####################################################################
	## Compiler Specific Flags
	####################################################################
	IF( "${CC_NAME}" MATCHES "gcc.*" )
    #MESSAGE( STATUS "adding extra LINK_FLAGS for ${CMAKE_CXX_COMPILER}" )
		SET_TARGET_PROPERTIES( ${tgt} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++" )
	ENDIF()
  
ENDMACRO()
