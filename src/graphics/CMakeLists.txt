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

INCLUDE_DIRECTORIES( . ../main ../third_party/png ../third_party/jpeg ../tiff++)

ADD_LIBRARY(y-graphics STATIC ${srcs} ${srcs_image} ${srcs_ops} )
INSTALL( TARGETS y-graphics ARCHIVE DESTINATION lib)

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
