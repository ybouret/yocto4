PROJECT(y-net)

FILE( GLOB net_decl yocto/net/*.hpp)
FILE( GLOB net_impl yocto/net/*.cpp)
SET( net_srcs ${net_decl} ${net_impl} )
INSTALL( FILES ${net_decl} DESTINATION include/yocto/net )
SOURCE_GROUP( net FILES ${net_srcs} )

FILE( GLOB sys_decl yocto/net/sys/*.hpp)
FILE( GLOB sys_impl yocto/net/sys/*.cpp)
SET( sys_srcs ${sys_decl} ${sys_impl} )
INSTALL( FILES ${sys_decl} DESTINATION include/yocto/net/sys )
SOURCE_GROUP( net/sys FILES ${sys_srcs} )

FILE( GLOB ip_decl yocto/net/ip/*.hpp)
FILE( GLOB ip_impl yocto/net/ip/*.cpp)
SET( ip_srcs ${ip_decl} ${ip_impl} )
INSTALL( FILES ${ip_decl} DESTINATION include/yocto/net/ip )
SOURCE_GROUP( net/ip FILES ${ip_srcs} )

SET( decl ${net_decl} ${sys_decl} ${ip_decl} )
SET( srcs ${net_srcs} ${sys_srcs} ${ip_srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY(y-net STATIC ${srcs} )

INSTALL( TARGETS y-net ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
