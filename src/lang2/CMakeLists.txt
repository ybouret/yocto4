
FILE(GLOB lang_decl yocto/lang/*.hpp)
FILE(GLOB lang_impl yocto/lang/*.cpp)
SET(lang_srcs ${lang_decl} ${lang_impl})
INSTALL(FILES ${lang_decl} DESTINATION include/yocto/lang )
SOURCE_GROUP(lang FILES ${lang_srcs})

FILE(GLOB pat_decl yocto/lang/pattern/*.hpp)
FILE(GLOB pat_impl yocto/lang/pattern/*.cpp)
SET(pat_srcs ${pat_decl} ${pat_impl})
INSTALL(FILES ${pat_decl} DESTINATION include/yocto/lang/pattern )
SOURCE_GROUP(lang/pattern FILES ${pat_srcs})

#private sources, not installed
FILE(GLOB rx_decl yocto/lang/regexp/*.hxx)
FILE(GLOB rx_impl yocto/lang/regexp/*.cxx)
SET(rx_srcs ${rx_decl} ${rx_impl})
SOURCE_GROUP(lang/regexp FILES ${rx_srcs})

FILE(GLOB lex_decl yocto/lang/lexical/*.hpp)
FILE(GLOB lex_impl yocto/lang/lexical/*.cpp)
SET(lex_srcs ${lex_decl} ${lex_impl})
INSTALL(FILES ${lex_decl} DESTINATION include/yocto/lang/lexical )
SOURCE_GROUP(lang/lexical FILES ${lex_srcs})

INCLUDE_DIRECTORIES(../main .)
ADD_LIBRARY(y-lang2 STATIC ${lang_srcs} ${pat_srcs} ${rx_srcs} ${lex_srcs})

#INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
