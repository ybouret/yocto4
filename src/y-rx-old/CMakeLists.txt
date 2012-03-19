PROJECT(y-rx)

FILE( GLOB rx_decl yocto/rx/*.hpp)
FILE( GLOB rx_impl yocto/rx/*.cpp)
SET( rx_srcs ${rx_decl} ${rx_impl} )
INSTALL( FILES ${rx_decl} DESTINATION include/yocto/rx )
SOURCE_GROUP( rx FILES ${rx_srcs} )

FILE( GLOB pat_decl yocto/rx/pattern/*.hpp)
FILE( GLOB pat_impl yocto/rx/pattern/*.cpp)
SET( pat_srcs ${pat_decl} ${pat_impl} )
INSTALL( FILES ${pat_decl} DESTINATION include/yocto/rx/pattern )
SOURCE_GROUP( rx/pattern FILES ${pat_srcs} )


FILE( GLOB lex_decl yocto/rx/lexical/*.hpp)
FILE( GLOB lex_impl yocto/rx/lexical/*.cpp)
SET( lex_srcs ${lex_decl} ${lex_impl} )
INSTALL( FILES ${lex_decl} DESTINATION include/yocto/rx/lexical )
SOURCE_GROUP( rx/lexical FILES ${lex_srcs} )

#FILE( GLOB syn_decl yocto/rx/syntactic/*.hpp)
#FILE( GLOB syn_impl yocto/rx/syntactic/*.cpp)
#SET( syn_srcs ${syn_decl} ${syn_impl} )
#INSTALL( FILES ${syn_decl} DESTINATION include/yocto/rx/syntactic )
#SOURCE_GROUP( rx/syntactic FILES ${syn_srcs} )

FILE( GLOB syn_decl yocto/rx/syntax/*.hpp)
FILE( GLOB syn_impl yocto/rx/syntax/*.cpp)
SET( syn_srcs ${syn_decl} ${syn_impl} )
INSTALL( FILES ${syn_decl} DESTINATION include/yocto/rx/syntax )
SOURCE_GROUP( rx/syntax FILES ${syn_srcs} )

INCLUDE_DIRECTORIES( . ../y-main )


ADD_LIBRARY( y-rx STATIC ${rx_srcs} ${pat_srcs} ${lex_srcs} ${syn_srcs} )
INSTALL( TARGETS y-rx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

