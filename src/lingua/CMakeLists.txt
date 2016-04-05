
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

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY(y-lingua STATIC ${srcs} ${pattern_srcs})

ADD_SUBDIRECTORY(tests)

