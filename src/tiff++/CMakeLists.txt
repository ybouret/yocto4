PROJECT(tiff++)

FIND_PACKAGE(TIFF)

IF(TIFF_FOUND)

	FILE( GLOB decl yocto/tiff++/*.hpp)
	FILE( GLOB impl yocto/tiff++/*.cpp)
	SET( srcs ${decl} ${impl} )
	INSTALL( FILES ${decl} DESTINATION include/yocto/tiff++ )
	SOURCE_GROUP( tiff++ FILES ${srcs} )

	INCLUDE_DIRECTORIES( . ../main  ${TIFF_INCLUDE_DIR})

	ADD_LIBRARY( y-tiff++ STATIC ${srcs} )
	INSTALL( TARGETS y-tiff++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

ENDIF(TIFF_FOUND)

