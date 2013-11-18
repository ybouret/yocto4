PROJECT(y-z++)

FILE( GLOB decl yocto/z++/*.hpp)
FILE( GLOB impl yocto/z++/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/z++ )
SOURCE_GROUP( z++ FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-z )

ADD_LIBRARY( y-z++ STATIC ${srcs} )
INSTALL( TARGETS y-z++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

