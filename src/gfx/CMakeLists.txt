FILE(GLOB decl yocto/gfx/*.hpp)
FILE(GLOB impl yocto/gfx/*.cpp)
SET(srcs ${decl} ${impl})
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../third_party/png ../third_party/jpeg ../tiff++)

ADD_LIBRARY(y-gfx STATIC ${srcs})

ADD_SUBDIRECTORY(tests)


