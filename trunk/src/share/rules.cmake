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

