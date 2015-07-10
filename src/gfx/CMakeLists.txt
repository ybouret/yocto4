FILE(GLOB decl yocto/gfx/*.hpp)
FILE(GLOB impl yocto/gfx/*.cpp)
SET(srcs ${decl} ${impl} yocto/gfx/colors.hxx)
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

FILE(GLOB ramp_decl yocto/gfx/ramp/*.hpp)
FILE(GLOB ramp_impl yocto/gfx/ramp/*.cpp)
SET(ramp_srcs ${ramp_decl} ${ramp_impl})
INSTALL( FILES ${ramp_decl} DESTINATION include/yocto/gfx/ramp )
SOURCE_GROUP( gfx/ramp FILES ${ramp_srcs} )

FILE(GLOB image_decl yocto/gfx/image/*.hpp)
FILE(GLOB image_impl yocto/gfx/image/*.cpp)
SET(image_srcs ${image_decl} ${image_impl})
INSTALL( FILES ${image_decl} DESTINATION include/yocto/gfx/image )
SOURCE_GROUP( gfx/image FILES ${image_srcs} )

INCLUDE_DIRECTORIES( . ../main ../third_party/png ../third_party/jpeg ../tiff++)

ADD_LIBRARY(y-gfx STATIC ${srcs} ${ramp_srcs} ${image_srcs} )

ADD_SUBDIRECTORY(tests)


