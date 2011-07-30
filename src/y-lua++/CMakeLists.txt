PROJECT(y-lua++)

FILE( GLOB lua_decl yocto/lua/*.hpp)
FILE( GLOB lua_impl yocto/lua/*.cpp)
SET( lua_srcs ${lua_decl} ${lua_impl} )
INSTALL( FILES ${lua_decl} DESTINATION include/yocto/lua )
SOURCE_GROUP( lua FILES ${lua_srcs} )

INCLUDE_DIRECTORIES( . ../y-main  ../y-lua/yocto/lua )

ADD_LIBRARY( y-lua++ STATIC ${lua_srcs})
INSTALL( TARGETS y-lua++ ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

