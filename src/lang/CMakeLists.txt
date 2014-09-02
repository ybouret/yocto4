
FILE(GLOB lang_decl yocto/lang/*.hpp)
FILE(GLOB lang_impl yocto/lang/*.cpp)
SET(lang_srcs ${lang_decl} ${lang_impl})
INSTALL(FILES ${lang_decl} DESTINATION include/yocto/lang )
SOURCE_GROUP(lang FILES ${lang_srcs})

FILE(GLOB pattern_decl yocto/lang/pattern/*.hpp)
FILE(GLOB pattern_impl yocto/lang/pattern/*.cpp)
SET(pattern_srcs ${pattern_decl} ${pattern_impl})
INSTALL(FILES ${pattern_decl} DESTINATION include/yocto/lang/pattern)
SOURCE_GROUP(lang/pattern FILES ${pattern_srcs})

FILE(GLOB lexical_decl yocto/lang/lexical/*.hpp)
FILE(GLOB lexical_impl yocto/lang/lexical/*.cpp)
SET(lexical_srcs ${lexical_decl} ${lexical_impl})
INSTALL(FILES ${lexical_decl} DESTINATION include/yocto/lang/lexical)
SOURCE_GROUP(lang/lexical FILES ${lexical_srcs})

INCLUDE_DIRECTORIES(../main .)
ADD_LIBRARY(y-lang STATIC ${lang_srcs} ${pattern_srcs} ${lexical_srcs} )

ADD_SUBDIRECTORY(tests)

