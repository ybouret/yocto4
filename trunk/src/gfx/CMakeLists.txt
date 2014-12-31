PROJECT(gfx)

FILE( GLOB decl yocto/gfx/*.hpp)
FILE( GLOB impl yocto/gfx/*.cpp)

FILE( GLOB img_decl yocto/gfx/image/*.hpp )
FILE( GLOB img_impl yocto/gfx/image/*.cpp )

SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

SET( img_srcs ${img_decl} ${img_impl} )
INSTALL( FILES ${img_decl} DESTINATION include/yocto/gfx/image )
SOURCE_GROUP( gfx/image FILES ${img_srcs} )

INCLUDE_DIRECTORIES( . ../main ../third_party/png/yocto/png)

########################################################################
##
## Generating the library 
##
########################################################################
ADD_LIBRARY( y-gfx STATIC ${srcs} ${img_srcs}
)
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

