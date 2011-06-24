PROJECT(y-mpk)

FILE( GLOB decl yocto/mpk/*.hpp)
FILE( GLOB impl yocto/mpk/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION yocto/mpk )
SOURCE_GROUP( mpk FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-mpk STATIC ${srcs} )
INSTALL( TARGETS y-mpk ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

