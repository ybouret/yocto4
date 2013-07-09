PROJECT(y-gfx)

FILE( GLOB decl yocto/gfx/*.hpp)
FILE( GLOB impl yocto/gfx/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main)

ADD_LIBRARY( y-gfx STATIC ${srcs} )
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

