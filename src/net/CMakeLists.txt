
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


INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY(y-net STATIC
${net_srcs}
${addr_srcs}
yocto/net/sockaddr/ctor.hxx
${bsd_srcs} )

INSTALL( TARGETS y-net ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
