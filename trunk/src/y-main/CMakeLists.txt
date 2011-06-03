
FILE( GLOB top_decl yocto/*.hpp)
FILE( GLOB top_impl yocto/*.cpp)
SET( top_srcs ${top_decl} ${top_impl} )
INSTALL( FILES ${top_decl} DESTINATION include/yocto )

FILE( GLOB threading_decl yocto/threading/*.hpp)
FILE( GLOB threading_impl yocto/threading/*.cpp)
SET( threading_srcs ${threading_decl} ${threading_impl} )
INSTALL( FILES ${threading_decl} DESTINATION include/yocto/threading)
SOURCE_GROUP(threading FILES ${threading_srcs} )

SET( decl ${top_decl} ${threading_decl})
SET( srcs ${top_srcs} ${threading_srcs})

INCLUDE_DIRECTORIES( . )

ADD_LIBRARY(yocto STATIC ${srcs} )

INSTALL( TARGETS yocto ARCHIVE DESTINATION lib )

