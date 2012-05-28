PROJECT(y-lang)

FILE( GLOB decl yocto/lang/*.hpp)
FILE( GLOB impl yocto/lang/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lang )
SOURCE_GROUP( lang FILES ${srcs} )

FILE( GLOB lex_decl yocto/lang/lexical/*.hpp)
FILE( GLOB lex_impl yocto/lang/lexical/*.cpp)
SET( lex_srcs ${lex_decl} ${lex_impl} )
INSTALL( FILES ${lex_decl} DESTINATION include/yocto/lang/lexical )
SOURCE_GROUP( lang/lexical FILES ${lex_srcs} )

FILE( GLOB snx_decl yocto/lang/syntax/*.hpp)
FILE( GLOB snx_impl yocto/lang/syntax/*.cpp)
SET( snx_srcs ${snx_decl} ${snx_impl} )
INSTALL( FILES ${snx_decl} DESTINATION include/yocto/snx/lexical )
SOURCE_GROUP( lang/syntax FILES ${snx_srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-rx)

ADD_LIBRARY( y-lang STATIC ${srcs} ${lex_srcs} ${snx_srcs})
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

