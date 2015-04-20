
FILE(GLOB lang_decl yocto/lang/*.hpp)
FILE(GLOB lang_impl yocto/lang/*.cpp)
SET(lang_srcs ${lang_decl} ${lang_impl})
INSTALL(FILES ${lang_decl} DESTINATION include/yocto/lang )
SOURCE_GROUP(lang FILES ${lang_srcs})


INCLUDE_DIRECTORIES(../main .)
ADD_LIBRARY(y-lang2 STATIC ${lang_srcs})

#INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
