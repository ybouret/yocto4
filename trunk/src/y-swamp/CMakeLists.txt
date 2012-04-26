PROJECT(y-swamp)

FILE( GLOB decl yocto/swamp/*.hpp)
FILE( GLOB impl yocto/swamp/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/swamp )
SOURCE_GROUP( swamp FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-swamp STATIC ${srcs} )
INSTALL( TARGETS y-swamp ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

