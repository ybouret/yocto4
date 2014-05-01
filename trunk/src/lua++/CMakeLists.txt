PROJECT(lua++)

FILE( GLOB lua_decl yocto/lua/*.hpp)
FILE( GLOB lua_impl yocto/lua/*.cpp)
SET( lua_srcs ${lua_decl} ${lua_impl} )
INSTALL( FILES ${lua_decl} DESTINATION include/yocto/lua )
SOURCE_GROUP( lua FILES ${lua_srcs} )

FILE( GLOB xlua_decl yocto/lua/x/*.hpp)
FILE( GLOB xlua_impl yocto/lua/x/*.cpp)
SET( xlua_srcs ${xlua_decl} ${xlua_impl} )
INSTALL( FILES ${xlua_decl} DESTINATION include/yocto/lua/x )
SOURCE_GROUP( lua/x FILES ${xlua_srcs} )


INCLUDE_DIRECTORIES( . ../main  ../third_party/lua/yocto/lua )

ADD_LIBRARY( y-lua++ STATIC ${lua_srcs} ${xlua_srcs})
INSTALL( TARGETS y-lua++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

