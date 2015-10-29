FILE(GLOB decl yocto/graphix/*.hpp)
FILE(GLOB impl yocto/graphix/*.cpp)
SET(srcs ${impl} ${decl})
INSTALL( FILES ${decl} DESTINATION include/yocto/graphix )
SOURCE_GROUP( graphix FILES ${srcs} )

FILE(GLOB decl_image yocto/graphix/image/*.hpp)
FILE(GLOB impl_image yocto/graphix/image/*.cpp)
SET(srcs_image ${impl_image} ${decl_image})
INSTALL( FILES ${decl_image} DESTINATION include/yocto/graphix/image )
SOURCE_GROUP( graphix/image FILES ${srcs_image} )


INCLUDE_DIRECTORIES( . ../main ../third_party/png ../third_party/jpeg ../tiff++)

ADD_LIBRARY(y-graphix STATIC ${srcs} ${srcs_image} )

ADD_SUBDIRECTORY(tests)

