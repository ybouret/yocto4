
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



INCLUDE_DIRECTORIES(../main .)
ADD_LIBRARY(y-lang2 STATIC ${lang_srcs} ${pat_srcs})

#INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(tests)
