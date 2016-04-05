
FILE(GLOB decl yocto/lingua/*.hpp)
FILE(GLOB impl yocto/lingua/*.cpp)
SET(srcs ${decl} ${impl})
SOURCE_GROUP(lingua FILES ${srcs})

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY(y-lingua STATIC ${srcs} )

ADD_SUBDIRECTORY(tests)

