########################################################################
##
########################################################################
MESSAGE( STATUS "" )
MESSAGE( STATUS "---------------- <yocto>  ----------------" )
SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

########################################################################
##
########################################################################
EXEC_PROGRAM( uname ARGS "-m" OUTPUT_VARIABLE BUILD_MACHINE )
MESSAGE( STATUS "BUILD_MACHINE=${BUILD_MACHINE}" )

SET(YOCTO64 OFF)
IF( "${BUILD_MACHINE}" MATCHES ".*64" )
  SET(YOCTO64 ON)
  MESSAGE( STATUS "64-bits platform detected")
ELSE()
  MESSAGE( STATUS "32-bits platform detected" )
ENDIF()


MESSAGE( STATUS "CMAKE_SYSTEM_NAME= '${CMAKE_SYSTEM_NAME}'")
MESSAGE( STATUS "CMAKE_GENERATOR  = '${CMAKE_GENERATOR}'" )

SET(YOCTO_MACOSX OFF)
IF( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  SET(YOCTO_MACOSX ON)
ENDIF()

SET(YOCTO_LINUX OFF)
IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  SET(YOCTO_LINUX ON)
ENDIF()


SET(YOCTO_FREEBSD OFF)
IF( ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" )
  SET(YOCTO_FREEBSD ON)
ENDIF()

SET(YOCTO_OPENBSD OFF)
IF( ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD" )
  SET(YOCTO_OPENBSD ON)
ENDIF()


SET(YOCTO_SUNOS OFF)
IF( ${CMAKE_SYSTEM_NAME} MATCHES "SunOS" )
  SET(YOCTO_SUNOS ON)
ENDIF()

IF(YOCTO_LINUX OR YOCTO_SUNOS OR YOCTO_FREEBSD)
	SET(PIC_FLAGS "-fPIC")
ELSE()
	SET(PIC_FLAGS "")
ENDIF()

########################################################################
## tuning compilers flags
########################################################################
GET_FILENAME_COMPONENT(CC_NAME ${CMAKE_C_COMPILER} NAME )

SET(YOCTO_KNOWN_COMPILER OFF)

########################################################################
## GNU
########################################################################
IF( "${CC_NAME}" MATCHES "^gcc.*" )
  SET(YOCTO_GNU ON)
  SET(YOCTO_FAMILY "gnu")
  SET(YOCTO_KNOWN_COMPILER ON)
  MESSAGE( STATUS "Using GNU compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS} ${PIC_FLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g -O0" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions -Weffc++ ${MY_CXXFLAGS} ${PIC_FLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g -O0" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")

  EXEC_PROGRAM(${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE GCC_CORE_VERSION)
  STRING( REGEX MATCH    "[0-9]+\\.[0-9]+\\.[0-9]+" GCC_VERSION "${GCC_CORE_VERSION}" )

  STRING( REGEX REPLACE  "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" GCC_MAJOR "${GCC_VERSION}")
  STRING( REGEX REPLACE  "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" GCC_MINOR "${GCC_VERSION}")
  STRING( REGEX REPLACE  "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" GCC_PATCH "${GCC_VERSION}")
  MESSAGE( STATUS "GCC_VERSION=${GCC_MAJOR}.${GCC_MINOR}.${GCC_PATCH}" )
ENDIF()

########################################################################
## clang
########################################################################
IF( "${CC_NAME}" MATCHES "^clang.*" )
  SET(YOCTO_CLANG ON)
  SET(YOCTO_FAMILY "clang")
  SET(YOCTO_KNOWN_COMPILER ON)
  MESSAGE( STATUS "Using CLANG compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe ${MY_CFLAGS} ${PIC_FLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g -O0" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -fexceptions -Weffc++ ${MY_CXXFLAGS} ${PIC_FLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g -O0" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
ENDIF()



########################################################################
## Intel 
########################################################################
IF( "${CC_NAME}" MATCHES "^icc.*" )
  SET(YOCTO_INTEL ON)
  SET(YOCTO_FAMILY "intel")
  SET(YOCTO_KNOWN_COMPILER ON)

  MESSAGE( STATUS "Using Intel compilers" )
  
  SET(COMMON_C_FLAGS        "-Wall -pipe -wd981 ${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS} -g" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -O2 -DNDEBUG=1 -DFORTIFY_SOURCE=2")
  
  SET(COMMON_CXX_FLAGS        "-Wall -pipe -wd981 -fexceptions${MY_CXXFLAGS}" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS} -g" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O2 -DNDEBUG=1 -DFORTIFY_SOURCE=2")
ENDIF()

########################################################################
## PathScale
########################################################################
IF( "${CC_NAME}" MATCHES "^path.*" )
  SET(YOCTO_PATHSCALE ON)
  SET(YOCTO_FAMILY "path")
  SET(YOCTO_KNOWN_COMPILER ON)

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
  SET(YOCTO_MSC ON)
  SET(YOCTO_FAMILY "msc")
  SET(YOCTO_KNOWN_COMPILER ON)
  MESSAGE( STATUS "Using Microsoft cl.exe" )
  
  SET(COMMON_C_FLAGS        "${MY_CFLAGS}" )
  SET(CMAKE_C_FLAGS_DEBUG   "${COMMON_C_FLAGS}" )
  SET(CMAKE_C_FLAGS_RELEASE "${COMMON_C_FLAGS} -Ox -DNDEBUG=1")
  
  SET(COMMON_CXX_FLAGS        "${MY_CXXFLAGS} -EHsc" )
  SET(CMAKE_CXX_FLAGS_DEBUG   "${COMMON_CXX_FLAGS}" )
  SET(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -Ob2it -DNDEBUG=1")
  
ENDIF()

IF(NOT YOCTO_KNOWN_COMPILER)
	MESSAGE( FATAL_ERROR "UNKNOWN COMPILERS! '${CC_NAME}' " )
ENDIF()

########################################################################
# Common Flags
# Adjusting Linker Flags
########################################################################
MACRO(TARGET_LINK_YOCTO tgt)
	MESSAGE( STATUS "yocto --> ${tgt}" )

	####################################################################
	## Reverse order extra libraries
	####################################################################
	SET(YOCTO_HAS_NET  OFF)
	SET(YOCTO_HAS_LZMA OFF)
	FOREACH( extra ${ARGN} )
		TARGET_LINK_LIBRARIES( ${tgt} y-${extra} )
		
		STRING(COMPARE EQUAL "net"  "${extra}" YOCTO_HAS_NET)
		STRING(COMPARE EQUAL "lzma" "${extra}" YOCTO_HAS_LZMA)
		STRING(COMPARE EQUAL "tiff" "${extra}" YOCTO_HAS_TIFF)
		
	ENDFOREACH()
	
	

	####################################################################
	## Yocto Core C/C++
	####################################################################
	TARGET_LINK_LIBRARIES( ${tgt} yocto ${MY_LDFLAGS})

	####################################################################
	## Platform Specific Flags
	####################################################################
	IF( YOCTO_LINUX )
		TARGET_LINK_LIBRARIES( ${tgt} pthread rt)
	ENDIF()
	
	IF( YOCTO_FREEBSD )
		TARGET_LINK_LIBRARIES( ${tgt} pthread )
	ENDIF()
	
	IF( YOCTO_OPENBSD )
		TARGET_LINK_LIBRARIES( ${tgt} pthread )
	ENDIF()

	####################################################################
	## Compiler Specific Flags
	####################################################################
	IF( YOCTO_GNU AND WIN32)
		IF("${GCC_VERSION}" VERSION_GREATER "4.5.0" )
		TARGET_LINK_LIBRARIES( ${tgt} -static-libgcc -static-libstdc++ )
		ENDIF()
	ENDIF()
	
	IF( "${CMAKE_GENERATOR}" STREQUAL "Xcode" )
		TARGET_LINK_LIBRARIES( ${tgt} -w )
	ENDIF()
  
	IF( YOCTO_CLANG )
		TARGET_LINK_LIBRARIES( ${tgt} stdc++ ) #-flto 
	ENDIF()
  
	####################################################################
	## Network Specific Flags
	####################################################################
	IF(YOCTO_HAS_NET)
		MESSAGE( STATUS "${tgt} uses network libraries" )
		
		IF(WIN32)
			IF( "${CC_NAME}" MATCHES "gcc.*" )
				TARGET_LINK_LIBRARIES( ${tgt} ws2_32 )
			ENDIF()
		ENDIF()

		IF(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
			TARGET_LINK_LIBRARIES(${tgt} socket nsl)
		ENDIF()
	ENDIF()	
	
	####################################################################
	## LZMA/XZ Specific Flags
	####################################################################
	IF(YOCTO_HAS_LZMA)
		IF(YOCTO_SUNOS OR YOCTO_FREEBSD)
			TARGET_LINK_LIBRARIES(${tgt} md)
		ENDIF()
	ENDIF()
	
	IF(YOCTO_HAS_TIFF)
		TARGET_LINK_LIBRARIES(${tgt} y-z y-jpeg y-jbig)
		IF(NOT MSVC)
			TARGET_LINK_LIBRARIES(${tgt} y-lzma)
			IF(YOCTO_SUNOS OR YOCTO_FREEBSD)
			TARGET_LINK_LIBRARIES(${tgt} md)	
			ENDIF()
		ENDIF()
	ENDIF()
ENDMACRO()

FUNCTION(YOCTO_FILE2DATA source target)
ADD_CUSTOM_COMMAND(
OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${target}
COMMAND file2data ${CMAKE_CURRENT_SOURCE_DIR}/${source} ${CMAKE_CURRENT_SOURCE_DIR}/${target}
DEPENDS file2data ${CMAKE_CURRENT_SOURCE_DIR}/${source}
COMMENT "[File2Data] ${source} => ${target}"
)

ENDFUNCTION(YOCTO_FILE2DATA)


MESSAGE( STATUS "---------------- </yocto> ----------------" )
MESSAGE( STATUS "" )
