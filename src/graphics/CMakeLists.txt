ADD_EXECUTABLE(make_ramp make_ramp.cpp)
TARGET_LINK_YOCTO(make_ramp)


FILE(GLOB decl yocto/graphics/*.hpp)
FILE(GLOB impl yocto/graphics/*.cpp)
SET(srcs ${impl} ${decl})
INSTALL( FILES ${decl} DESTINATION include/yocto/graphics )
SOURCE_GROUP( graphics FILES ${srcs} )

FILE(GLOB decl_image yocto/graphics/image/*.hpp)
FILE(GLOB impl_image yocto/graphics/image/*.cpp)
SET(srcs_image ${impl_image} ${decl_image})
INSTALL( FILES ${decl_image} DESTINATION include/yocto/graphics/image )
SOURCE_GROUP( graphics/image FILES ${srcs_image} )

FILE(GLOB decl_ops yocto/graphics/ops/*.hpp)
FILE(GLOB impl_ops yocto/graphics/ops/*.cpp)
SET(srcs_ops ${impl_ops} ${decl_ops})
INSTALL( FILES ${decl_ops} DESTINATION include/yocto/graphics/ops )
SOURCE_GROUP( graphics/ops FILES ${srcs_ops} )

FILE(GLOB decl_ramp yocto/graphics/ramp/*.hpp)
FILE(GLOB impl_ramp yocto/graphics/ramp/*.cpp)
SET(srcs_ramp ${impl_ramp} ${decl_ramp})
INSTALL( FILES ${decl_ramp} DESTINATION include/yocto/graphics/ramp )
SOURCE_GROUP( graphics/ramp FILES ${srcs_ramp} )

FILE(GLOB decl_piv yocto/graphics/piv/*.hpp)
FILE(GLOB impl_piv yocto/graphics/piv/*.cpp)
SET(srcs_piv ${impl_piv} ${decl_piv})
INSTALL( FILES ${decl_piv} DESTINATION include/yocto/graphics/piv )
SOURCE_GROUP( graphics/piv FILES ${srcs_piv} )


INCLUDE_DIRECTORIES( . ../main ../mk ../third_party/png ../third_party/jpeg ../tiff++)

ADD_LIBRARY(y-graphics STATIC ${srcs} ${srcs_image} ${srcs_ops} ${srcs_ramp} ${srcs_piv} )
INSTALL( TARGETS y-graphics ARCHIVE DESTINATION lib)

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
