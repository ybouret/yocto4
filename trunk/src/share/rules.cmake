########################################################################
##
########################################################################

SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

########################################################################
##
########################################################################

GET_FILENAME_COMPONENT(CXX_NAME ${CMAKE_CXX_COMPILER} NAME )

IF( "${CXX_NAME}" MATCHES "g[+][+].*" )
  MESSAGE( STATUS "Using GNU compilers" )
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS}" )

  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions -Weffc++ ${MY_CXXFLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1")

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
	IF( "${CXX_NAME}" MATCHES "g[+][+].*" )
		SET_TARGET_PROPERTIES( ${tgt} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++" )
	ENDIF()
ENDMACRO()