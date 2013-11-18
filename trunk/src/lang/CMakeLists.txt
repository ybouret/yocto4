PROJECT(y-lang)

FILE( GLOB decl yocto/lang/*.hpp)
FILE( GLOB impl yocto/lang/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lang )
SOURCE_GROUP( lang FILES ${srcs} )

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

FILE( GLOB lex_decl yocto/lang/lexical/*.hpp)
FILE( GLOB lex_impl yocto/lang/lexical/*.cpp)
SET( lex_srcs ${lex_decl} ${lex_impl} )
INSTALL( FILES ${lex_decl} DESTINATION include/yocto/lang/lexical )
SOURCE_GROUP( lang/lexical FILES ${lex_srcs} )

FILE( GLOB snx_decl yocto/lang/syntax/*.hpp)
FILE( GLOB snx_impl yocto/lang/syntax/*.cpp)
SET( snx_srcs ${snx_decl} ${snx_impl} )
INSTALL( FILES ${snx_decl} DESTINATION include/yocto/syntax/lexical )
SOURCE_GROUP( lang/syntax FILES ${snx_srcs} )



INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-lang STATIC ${srcs} ${rx_srcs} ${pat_srcs} ${lex_srcs} ${snx_srcs})
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

