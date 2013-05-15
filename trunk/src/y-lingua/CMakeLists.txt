PROJECT(y-lang)

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

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-lingua STATIC ${srcs} ${pat_srcs} ${lex_srcs})
INSTALL( TARGETS y-lingua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

