PROJECT(z++)

FILE( GLOB decl yocto/z++/*.hpp)
FILE( GLOB impl yocto/z++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/z++ )
SOURCE_GROUP( z++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../third_party/z )

ADD_LIBRARY( y-z++ STATIC ${srcs} )
INSTALL( TARGETS y-z++ ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

