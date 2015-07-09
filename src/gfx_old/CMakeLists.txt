FILE( GLOB decl yocto/gfx/*.hpp)
FILE( GLOB impl yocto/gfx/*.cpp)



SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

FILE( GLOB img_decl yocto/gfx/image/*.hpp )
FILE( GLOB img_impl yocto/gfx/image/*.cpp )

SET( img_srcs ${img_decl} ${img_impl} )
INSTALL( FILES ${img_decl} DESTINATION include/yocto/gfx/image )
SOURCE_GROUP( gfx/image FILES ${img_srcs} )

FILE( GLOB ops_decl yocto/gfx/ops/*.hpp )
FILE( GLOB ops_impl yocto/gfx/ops/*.cpp )

SET( ops_srcs ${ops_decl} ${ops_impl} )
INSTALL( FILES ${ops_decl} DESTINATION include/yocto/gfx/ops )
SOURCE_GROUP( gfx/ops FILES ${ops_srcs} )

FILE( GLOB ramp_decl yocto/gfx/ramp/*.hpp )
FILE( GLOB ramp_impl yocto/gfx/ramp/*.cpp )

SET( ramp_srcs ${ramp_decl} ${ramp_impl} )
INSTALL( FILES ${ramp_decl} DESTINATION include/yocto/gfx/ramp )
SOURCE_GROUP( gfx/ramp FILES ${ramp_srcs} )

INCLUDE_DIRECTORIES( . ../main ../third_party/png ../third_party/jpeg ../tiff++)

########################################################################
##
## Generating the library 
##
########################################################################
ADD_LIBRARY( y-gfx_old STATIC ${srcs} ${img_srcs} ${ops_srcs} ${ramp_srcs}
)
#INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

ADD_EXECUTABLE(data2ramp data2ramp.cpp)
TARGET_LINK_YOCTO(data2ramp)

INSTALL( TARGETS data2ramp RUNTIME DESTINATION bin )
