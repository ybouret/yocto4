
SET(XZ_VERSION "5.2.2")
SET(XZ_WORKDIR "${CMAKE_CURRENT_SOURCE_DIR}/xz-${XZ_VERSION}")
SET(XZ_ARCHIVE "${XZ_WORKDIR}.tar.gz")	

SET(XZ_INSTALL "${CMAKE_CURRENT_SOURCE_DIR}/sdk-${CC_NAME}")
	
FUNCTION(XZ_IMPORT)
	
	
	IF(EXISTS "${XZ_ARCHIVE}")
		# already downloaded
		MESSAGE( STATUS "[XZ] found ${XZ_ARCHIVE}")
	ELSE()
		MESSAGE( STATUS "[XZ] downloading ${XZ_ARCHIVE}")
		FILE(DOWNLOAD "http://tukaani.org/xz/${XZ_ARCHIVE}" "${XZ_FULL_ID}" TIMEOUT 10 SHOW_PROGRESS)
	ENDIF()
	
	IF(EXISTS "${XZ_WORKDIR}")
		MESSAGE( STATUS "[XZ] already extracted" )
	ELSE()
		MESSAGE( STATUS "[XZ] extracting" )
		EXECUTE_PROCESS( COMMAND cmake -E tar xfz  "${XZ_ARCHIVE}" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	ENDIF()
	
	IF(APPLE)
		EXECUTE_PROCESS( COMMAND ./configure --prefix=${XZ_INSTALL} --enable-threads=no --disable-xz --disable-xzdec --disable-lzmadec --disable-lzmainfo --disable-lzma-links --disable-scripts --disable-doc --disable-shared --disable-nls --disable-rpath  
			WORKING_DIRECTORY ${XZ_WORKDIR} )
		EXECUTE_PROCESS( COMMAND make install WORKING_DIRECTORY ${XZ_WORKDIR} )
	ENDIF()
	
ENDFUNCTION(XZ_IMPORT)

XZ_IMPORT()
