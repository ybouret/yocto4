
STRING(TOLOWER "${CMAKE_SYSTEM_NAME}" XZ_SYSTEM_NAME)
SET(XZ_SETUP   "${CMAKE_CURRENT_SOURCE_DIR}/${XZ_SYSTEM_NAME}-${YOCTO_FAMILY}.cmake")
SET(XZ_CONFIG  "${CMAKE_CURRENT_SOURCE_DIR}/config-${XZ_SYSTEM_NAME}-${YOCTO_FAMILY}.h")
SET(XZ_OUTDIR  "${CMAKE_CURRENT_SOURCE_DIR}")
SET(XZ_INCDIR  "${XZ_OUTDIR}/include")
SET(XZ_SRCDIR  "${XZ_OUTDIR}/src")
SET(XZ_TSTDIR  "${XZ_OUTDIR}/tests")


SET(XZ_VERSION "5.2.2")
SET(XZ_WORKDIR "${CMAKE_CURRENT_BINARY_DIR}/xz-${XZ_VERSION}")
SET(XZ_ARCHIVE "xz-${XZ_VERSION}.tar.gz")
SET(XZ_TARBALL "${CMAKE_CURRENT_BINARY_DIR}/${XZ_ARCHIVE}")
SET(XZ_URL     "http://tukaani.org/xz/${XZ_ARCHIVE}")
SET(XZ_TARGET  "${CMAKE_CURRENT_BINARY_DIR}/sandbox")
SET(XZ_LIBRARY "${XZ_TARGET}/lib/liblzma.a")

MESSAGE( STATUS "XZ: setup  is '${XZ_SETUP}'")
MESSAGE( STATUS "XZ: config is '${XZ_CONFIG}'")

########################################################################
##
## script to build xz when possible
##
########################################################################
FUNCTION(XZ_BUILD)
	
	#check tarball
	IF(EXISTS ${XZ_TARBALL})
		MESSAGE( STATUS "XZ: found tarball")
	ELSE()
		MESSAGE( STATUS "XZ: downloading")
		FILE( DOWNLOAD ${XZ_URL} ${XZ_TARBALL} TIMEOUT 10 SHOW_PROGRESS )
	ENDIF()
	
	#check directory
	IF(IS_DIRECTORY ${XZ_WORKDIR})
		MESSAGE( STATUS "XZ: found directory"    )
	ELSE()
		MESSAGE( STATUS "XZ: extracting tarball" )
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E tar xfz ${XZ_TARBALL} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	ENDIF()
	
	#check sandbox was built
	IF(IS_DIRECTORY ${XZ_TARGET})
		MESSAGE( STATUS "XZ: xz was built")
	ELSE()
		MESSAGE( STATUS "XZ: configuring" )
		EXECUTE_PROCESS( COMMAND 
		./configure
		--prefix=${XZ_TARGET}
		 --enable-threads=no 
		 --disable-xz
		  --disable-xzdec 
		  --disable-lzmadec 
		  --disable-lzmainfo 
		  --disable-lzma-links 
		  --disable-scripts
		   --disable-doc 
		   --disable-shared 
		   --disable-nls 
		   --disable-rpath 
		   --disable-assembler
		   WORKING_DIRECTORY ${XZ_WORKDIR})
		MESSAGE( STATUS "XZ: build and install" )
		EXECUTE_PROCESS(COMMAND make install WORKING_DIRECTORY ${XZ_WORKDIR})
	ENDIF()
	
	#extracting object name from archive
	IF(NOT EXISTS ${XZ_LIBRARY})
		MESSAGE( FATAL_ERROR "XZ: connot find ${XZ_LIBRARY}")
	ENDIF()
	MESSAGE( STATUS "XZ: analyzing library content" )
	EXECUTE_PROCESS( COMMAND
		ar -t ${XZ_LIBRARY}
		OUTPUT_FILE       raw_content.log
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	
	#cleanup
	MESSAGE( STATUS "XZ: extracting object files")
	EXECUTE_PROCESS( COMMAND
	grep liblzma_la raw_content.log
	OUTPUT_FILE     content.log
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	
	#make a source file to copy
	MESSAGE( STATUS "XZ: converting into source files" )
	EXECUTE_PROCESS( COMMAND
	sed -e s/liblzma_la-// -e s/\\.o/\\.c/ content.log
	OUTPUT_FILE sources.log
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	
	#process implementation files
	FILE(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/sources.log XZ_SOURCES)
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E make_directory ${XZ_SRCDIR})

	FILE(WRITE ${XZ_SETUP} "SET(XZ_IMPL\n")
	FOREACH(src IN ITEMS ${XZ_SOURCES})
		MESSAGE( STATUS "XZ: ...processing ${src}" )
		FILE(APPEND ${XZ_SETUP} "src/${src}\n")
		EXECUTE_PROCESS( COMMAND find ${XZ_WORKDIR} -name ${src} OUTPUT_VARIABLE real_src OUTPUT_STRIP_TRAILING_WHITESPACE)
		#copy the real source
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${real_src} ${XZ_SRCDIR}/${src})
		
		#copy the real headers
		GET_FILENAME_COMPONENT(real_dir ${real_src} DIRECTORY)
		FILE(GLOB real_headers ${real_dir}/*.h)
		FOREACH(hdr IN ITEMS ${real_headers})
			EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${hdr} ${XZ_SRCDIR} )
		ENDFOREACH()
	ENDFOREACH()
	FILE(APPEND ${XZ_SETUP} ")\n")
	
	#copy public API and config.h
	MESSAGE( STATUS "XZ: copy LZMA public API" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E make_directory ${XZ_INCDIR}/lzma)
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XZ_TARGET}/include/lzma.h ${XZ_INCDIR})
	FILE(GLOB LZMA_DECL ${XZ_TARGET}/include/lzma/*.h)
	FOREACH(hdr IN ITEMS ${LZMA_DECL})
		GET_FILENAME_COMPONENT(hid ${hdr} NAME)
		MESSAGE( STATUS "XZ: copy lzma/${hid}")
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${hdr} ${XZ_INCDIR}/lzma)
	ENDFOREACH()
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XZ_WORKDIR}/config.h ${XZ_CONFIG})
	
	#copy tests
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E make_directory ${XZ_TSTDIR})
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XZ_WORKDIR}/doc/examples/01_compress_easy.c ${XZ_TSTDIR})
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XZ_WORKDIR}/doc/examples/02_decompress.c    ${XZ_TSTDIR})

ENDFUNCTION()


IF(NOT EXISTS ${XZ_SETUP})
	MESSAGE( STATUS "XZ: not found...")
	IF(NOT YOCTO_MSC)
		XZ_BUILD()
	ENDIF()
ENDIF()

IF(EXISTS ${XZ_SETUP})
INCLUDE(${XZ_SETUP})
#MESSAGE(STATUS "XZ_IMPL=${XZ_IMPL}" )
# select config.h
MESSAGE( STATUS "using ${XZ_CONFIG} as ${XZ_SRCDIR}/config.h" )
ADD_CUSTOM_COMMAND( OUTPUT  ${XZ_SRCDIR}/config.h
				    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XZ_CONFIG} ${XZ_SRCDIR}/config.h
				    DEPENDS ${XZ_CONFIG}
				    COMMENT "adjusting config.h")
				    
ADD_DEFINITIONS( -DHAVE_CONFIG_H )
INCLUDE_DIRECTORIES(./src ./include)
IF(YOCTO_GNU OR YOCTO_INTEL)
	ADD_COMPILE_OPTIONS("-std=c99")
ENDIF()

ADD_LIBRARY(y-lzma STATIC ${XZ_IMPL} ${XZ_SRCDIR}/config.h)

ADD_EXECUTABLE(y_lzma_compress   tests/01_compress_easy.c)
TARGET_LINK_LIBRARIES(y_lzma_compress y-lzma)
ADD_EXECUTABLE(y_lzma_decompress tests/02_decompress.c)
TARGET_LINK_LIBRARIES(y_lzma_decompress y-lzma)

IF(YOCTO_SUNOS OR YOCTO_FREEBSD)
	TARGET_LINK_LIBRARIES(y_lzma_compress md)
	TARGET_LINK_LIBRARIES(y_lzma_decompress md)
ENDIF()

ENDIF()


