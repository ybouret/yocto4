#ADD_EXECUTABLE(make_ramp make_ramp.cpp)
#TARGET_LINK_YOCTO(make_ramp)


FILE(GLOB decl yocto/gfx/*.hpp)
FILE(GLOB impl yocto/gfx/*.cpp)
SET(srcs ${impl} ${decl})
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

FILE(GLOB color_decl yocto/gfx/color/*.hpp)
FILE(GLOB color_impl yocto/gfx/color/*.cpp)
SET(color_srcs ${color_impl} ${color_decl})
INSTALL( FILES ${color_decl} DESTINATION include/yocto/gfx/color )
SOURCE_GROUP( gfx/color FILES ${color_srcs} )


FILE(GLOB ops_decl yocto/gfx/ops/*.hpp)
FILE(GLOB ops_impl yocto/gfx/ops/*.cpp)
SET(ops_srcs ${ops_impl} ${ops_decl})
INSTALL( FILES ${ops_decl} DESTINATION include/yocto/gfx/ops )
SOURCE_GROUP( gfx/ops FILES ${ops_srcs} )


INCLUDE_DIRECTORIES( . ../main ../mk
../third_party/png
../third_party/jpeg
../third_party/tiff/libtiff
${CMAKE_CURRENT_BINARY_DIR}/../third_party/tiff/libtiff)


ADD_LIBRARY(y-gfx STATIC ${srcs} ${color_srcs} ${ops_srcs})
TARGET_LINK_LIBRARIES(y-gfx y-tiff)

INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib)

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
