
FILE( GLOB net_decl yocto/net/*.hpp)
FILE( GLOB net_impl yocto/net/*.cpp)
SET( net_srcs ${net_decl} ${net_impl} )
INSTALL( FILES ${net_decl} DESTINATION include/yocto/net )
SOURCE_GROUP( net FILES ${net_srcs} )

FILE( GLOB addr_decl yocto/net/sockaddr/*.hpp )
FILE( GLOB addr_impl yocto/net/sockaddr/*.cpp )
SET( addr_srcs ${addr_decl} ${addr_impl}  )
INSTALL( FILES ${addr_decl} DESTINATION include/yocto/net/sockaddr )
SOURCE_GROUP( net/sockaddr FILES ${addr_srcs} yocto/net/sockaddr/ctor.hxx)

FILE( GLOB bsd_decl yocto/net/bsd/*.hpp )
FILE( GLOB bsd_impl yocto/net/bsd/*.cpp )
SET( bsd_srcs ${bsd_decl} ${bsd_impl}  )
INSTALL( FILES ${bsd_decl} DESTINATION include/yocto/net/bsd )
SOURCE_GROUP( net/bsd FILES ${bsd_srcs} )


FILE( GLOB socket_decl yocto/net/socket/*.hpp )
FILE( GLOB socket_impl yocto/net/socket/*.cpp )
SET( socket_srcs ${socket_decl} ${socket_impl}  )
INSTALL( FILES ${socket_decl} DESTINATION include/yocto/net/socket )
SOURCE_GROUP( net/socket FILES ${socket_srcs} )

FILE( GLOB io_decl yocto/net/io/*.hpp )
FILE( GLOB io_impl yocto/net/io/*.cpp )
SET( io_srcs ${io_decl} ${io_impl}  )
INSTALL( FILES ${io_decl} DESTINATION include/yocto/net/io )
SOURCE_GROUP( net/io FILES ${io_srcs} )

FILE( GLOB tcp_decl yocto/net/tcp/*.hpp )
FILE( GLOB tcp_impl yocto/net/tcp/*.cpp )
SET( tcp_srcs ${tcp_decl} ${tcp_impl}  )
INSTALL( FILES ${tcp_decl} DESTINATION include/yocto/net/tcp )
SOURCE_GROUP( net/tcp FILES ${tcp_srcs} )



INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY(y-net STATIC
${net_srcs}
${addr_srcs}
yocto/net/sockaddr/ctor.hxx
${bsd_srcs} 
${socket_srcs}
${io_srcs}
${tcp_srcs}
)

INSTALL( TARGETS y-net ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
