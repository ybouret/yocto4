PROJECT(y-gfx)

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

INCLUDE_DIRECTORIES( . ../y-main ../y-jpeg)

########################################################################
##
## Generating the named rgb colors
##
########################################################################
ADD_EXECUTABLE(named-rgb EXCLUDE_FROM_ALL named-rgb.cpp)
TARGET_LINK_YOCTO(named-rgb)


ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
        COMMAND named-rgb ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.txt ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.txt ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.cpp
)

########################################################################
##
## Generating the color ramps
##
########################################################################
ADD_EXECUTABLE(build-ramps EXCLUDE_FROM_ALL build-ramps.cpp)
TARGET_LINK_YOCTO(build-ramps)

FILE( GLOB ramps ramps/*.dat )
MESSAGE( STATUS "ramps are ${ramps}" )

########################################################################
##
## Generating the library 
##
########################################################################
SET( ramp_srcs ${ramp_decl} ${ramp_impl} )
INSTALL( FILES ${ramp_decl} DESTINATION include/yocto/gfx/ramp )
SOURCE_GROUP( gfx/ramp FILES ${ramp_srcs} )

ADD_LIBRARY( y-gfx STATIC 	${srcs}
				${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
				${img_srcs}
				${ramp_srcs}
)
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

