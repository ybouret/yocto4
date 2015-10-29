FILE(GLOB decl yocto/graphix/*.hpp)
FILE(GLOB impl yocto/graphix/*.cpp)
SET(srcs ${impl} ${decl})
INSTALL( FILES ${decl} DESTINATION include/yocto/graphix )
SOURCE_GROUP( graphix FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../third_party/png ../third_party/jpeg ../tiff++)

ADD_LIBRARY(y-graphix STATIC ${srcs} ${ramp_srcs} ${image_srcs} ${ops_srcs} )

ADD_SUBDIRECTORY(tests)

