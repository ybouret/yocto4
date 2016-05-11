
STRING(TOLOWER "${CMAKE_SYSTEM_NAME}" XZ_SYSTEM_NAME)
SET(XZ_SETUP   "${CMAKE_CURRENT_SOURCE_DIR}/${XZ_SYSTEM_NAME}-${YOCTO_FAMILY}.cmake")
SET(XZ_CONFIG  "${CMAKE_CURRENT_SOURCE_DIR}/config-${XZ_SYSTEM_NAME}-${YOCTO_FAMILY}.h")
SET(XZ_OUTDIR  "${CMAKE_CURRENT_SOURCE_DIR}")
SET(XZ_INCDIR  "${XZ_OUTDIR}/include")

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
	
	#copy public API
	MESSAGE( STATUS "XZ: copy LZMA public API" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E make_directory ${XZ_INCDIR}/lzma)
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy ${XZ_TARGET}/include/lzma.h ${XZ_INCDIR})
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy ${XZ_TARGET}/include/lzma/*.h ${XZ_INCDIR}/lzma)

ENDFUNCTION()


IF(EXISTS ${XZ_SETUP})
	MESSAGE( STATUS "XZ: found..." )
ELSE()
	MESSAGE( STATUS "XZ: not found...")
	IF(NOT YOCTO_MSC)
		XZ_BUILD()
	ENDIF()
ENDIF()

