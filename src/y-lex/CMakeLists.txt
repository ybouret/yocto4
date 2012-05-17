PROJECT(y-lex)

FILE( GLOB decl yocto/lexical/*.hpp)
FILE( GLOB impl yocto/lexical/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lexical )
SOURCE_GROUP( lexical FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-rx)

ADD_LIBRARY( y-lex STATIC ${srcs} )
INSTALL( TARGETS y-lex ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

