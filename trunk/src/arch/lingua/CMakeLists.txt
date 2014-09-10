PROJECT(lingua)

FILE( GLOB decl yocto/lingua/*.hpp)
FILE( GLOB impl yocto/lingua/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lingua )
SOURCE_GROUP( lingua FILES ${srcs} )

FILE( GLOB pat_decl yocto/lingua/pattern/*.hpp )
FILE( GLOB pat_impl yocto/lingua/pattern/*.cpp )
SET( pat_srcs ${pat_decl} ${pat_impl} )
INSTALL( FILES ${pat_decl} DESTINATION include/yocto/lingua/pattern )
SOURCE_GROUP( lingua/pattern FILES ${pat_srcs} )

FILE( GLOB lex_decl yocto/lingua/lexical/*.hpp )
FILE( GLOB lex_impl yocto/lingua/lexical/*.cpp )
SET( lex_srcs ${lex_decl} ${lex_impl} )
INSTALL( FILES ${lex_decl} DESTINATION include/yocto/lingua/lexical )
SOURCE_GROUP( lingua/lexical FILES ${lex_srcs} )

FILE( GLOB syn_decl yocto/lingua/syntax/*.hpp )
FILE( GLOB syn_impl yocto/lingua/syntax/*.cpp )
SET( syn_srcs ${syn_decl} ${syn_impl} )
INSTALL( FILES ${syn_decl} DESTINATION include/yocto/lingua/syntax )
SOURCE_GROUP( lingua/syntax FILES ${syn_srcs} )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-lingua STATIC ${srcs} ${pat_srcs} ${lex_srcs} ${syn_srcs})
INSTALL( TARGETS y-lingua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

