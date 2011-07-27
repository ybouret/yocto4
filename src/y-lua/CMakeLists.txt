PROJECT(y-lua)

FILE( GLOB lua_decl yocto/lua/*.h*)
FILE( GLOB lua_impl yocto/lua/*.c*)
SET( lua_srcs ${lua_decl} ${lua_impl} )
INSTALL( FILES ${lua_decl} DESTINATION yocto/lua )
SOURCE_GROUP( lua FILES ${lua_srcs} )

INCLUDE_DIRECTORIES( . ../y-main ./src/inc ./yocto/lua )

ADD_LIBRARY( y-lua STATIC ${lua_srcs})
INSTALL( TARGETS y-lua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

