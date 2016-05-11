
STRING(TOLOWER "${CMAKE_SYSTEM_NAME}" XZ_SYSTEM_NAME)
SET(XZ_SETUP   "${CMAKE_CURRENT_SOURCE_DIR}/${XZ_SYSTEM_NAME}-${YOCTO_FAMILY}.cmake")
SET(XZ_CONFIG  "${CMAKE_CURRENT_SOURCE_DIR}/config-${XZ_SYSTEM_NAME}-${YOCTO_FAMILY}.h")
SET(XZ_SRCDIR  "${CMAKE_CURRENT_SOURCE_DIR}/xz")

SET(XZ_VERSION "5.2.2")
SET(XZ_WORKDIR "${CMAKE_CURRENT_SOURCE_DIR}/xz-${XZ_VERSION}")
SET(XZ_ARCHIVE "xz-${XZ_VERSION}.tar.gz")
SET(XZ_TARBALL "${CMAKE_CURRENT_SOURCE_DIR}/${XZ_ARCHIVE}")
SET(XZ_URL     "http://tukaani.org/xz/${XZ_ARCHIVE}")
SET(XZ_TARGET  "${CMAKE_CURRENT_SOURCE_DIR}/sandbox")

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
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E tar xfz ${XZ_TARBALL} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
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
	ENDIF()
	
ENDFUNCTION()


IF(EXISTS ${XZ_SETUP})
	MESSAGE( STATUS "XZ: found..." )
ELSE()
	MESSAGE( STATUS "XZ: not found...")
	IF(NOT YOCTO_MSC)
		XZ_BUILD()
	ENDIF()
ENDIF()

