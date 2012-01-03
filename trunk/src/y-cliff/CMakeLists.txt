PROJECT(y-cliff)

FILE( GLOB cliff_decl yocto/cliff/*.hpp)
FILE( GLOB cliff_impl yocto/cliff/*.cpp)
SET( cliff_srcs ${cliff_decl} ${cliff_impl} )
INSTALL( FILES ${cliff_decl} DESTINATION include/yocto/cliff )
SOURCE_GROUP( cliff FILES ${cliff_srcs} )


INCLUDE_DIRECTORIES( . ../y-main )


ADD_LIBRARY( y-cliff STATIC ${cliff_srcs} )
INSTALL( TARGETS y-cliff ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

