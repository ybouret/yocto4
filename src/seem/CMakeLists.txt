FILE( GLOB decl yocto/seem/*.hpp )
FILE( GLOB impl yocto/seem/*.cpp )
SET( srcs ${decl} ${impl} )

INSTALL( FILES ${decl} DESTINATION include/yocto/seem )
SOURCE_GROUP( seem FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../lingua)

ADD_LIBRARY(y-seem STATIC  ${srcs} )
	
INSTALL( TARGETS y-seem ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
