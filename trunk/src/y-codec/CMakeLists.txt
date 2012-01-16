PROJECT(y-codec)

FILE( GLOB decl yocto/codec/*.hpp)
FILE( GLOB impl yocto/codec/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/codec )
SOURCE_GROUP( codec FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-codec STATIC ${srcs} )
INSTALL( TARGETS y-codec ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

