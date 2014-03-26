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

INCLUDE_DIRECTORIES( . ../main ../jpeg)

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
SET(builtin_ramp_decl)
SET(builtin_ramp_srcs)

FOREACH(f ${ramps})
	#MESSAGE( STATUS "${f}" )
	GET_FILENAME_COMPONENT(r ${f} NAME_WE )
	SET(r_com "${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/ramp/r${r}")
	SET(r_hdr "${r_com}.hpp")
	SET(r_src "${r_com}.cpp")
	ADD_CUSTOM_COMMAND(
		OUTPUT ${r_hdr} ${r_src}
		COMMAND build-ramps ${f} ${r_com}
		DEPENDS ${f} ${CMAKE_CURRENT_SOURCE_DIR}/build-ramps.cpp
		)
	LIST( APPEND builtin_ramp_decl ${r_hdr} )
	LIST( APPEND builtin_ramp_srcs ${r_src} )
ENDFOREACH()


########################################################################
##
## Generating the library 
##
########################################################################
SET( ramp_decl yocto/gfx/ramp/types.hpp ${builtin_ramp_decl})
SET( ramp_impl yocto/gfx/ramp/types.cpp ${builtin_ramp_srcs})
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

