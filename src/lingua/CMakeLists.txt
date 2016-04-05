
FILE(GLOB decl yocto/lingua/*.hpp)
FILE(GLOB impl yocto/lingua/*.cpp)
SET(srcs ${decl} ${impl})
SOURCE_GROUP(lingua FILES ${srcs})
INSTALL(FILES ${decl} DESTINATION include/yocto/lingua)

FILE(GLOB pattern_decl yocto/lingua/pattern/*.hpp)
FILE(GLOB pattern_impl yocto/lingua/pattern/*.cpp)
SET(pattern_srcs ${pattern_decl} ${pattern_impl} )
SOURCE_GROUP(lingua/pattern FILES ${pattern_srcs} )
INSTALL(FILES ${pattern_decl} DESTINATION include/yocto/lang/pattern )

FILE(GLOB regexp_decl yocto/lingua/regexp/*.hpp)
FILE(GLOB regexp_impl yocto/lingua/regexp/*.cpp)
SET(regexp_srcs ${regexp_decl} ${regexp_impl} )
SOURCE_GROUP(lingua/regexp FILES ${regexp_srcs} )
INSTALL(FILES ${regexp_decl} DESTINATION include/yocto/lang/regexp )

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY(y-lingua STATIC ${srcs} ${pattern_srcs} ${regexp_srcs})

ADD_SUBDIRECTORY(tests)

