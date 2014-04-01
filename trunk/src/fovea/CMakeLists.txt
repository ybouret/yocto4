PROJECT(fovea)


FILE( GLOB decl yocto/fovea/*.hpp )
FILE( GLOB srcs yocto/fovea/*.cpp )
SET( srcs ${decl} ${srcs} )

INSTALL( FILES ${decl} DESTINATION include/yocto/fovea )
SOURCE_GROUP( fovea FILES ${srcs} )

FILE( GLOB mesh_decl yocto/fovea/mesh/*.hpp )
FILE( GLOB mesh_impl yocto/fovea/mesh/*.cpp )
SET( mesh_srcs ${mesh_decl} ${mesh_impl} )

INSTALL( FILES ${mesh_decl} DESTINATION include/yocto/fovea/mesh )
SOURCE_GROUP( fovea/mesh FILES ${mesh_srcs} )

FILE( GLOB shape_decl yocto/fovea/shape/*.hpp )
FILE( GLOB shape_impl yocto/fovea/shape/*.cpp )
SET( shape_srcs ${shape_decl} ${shape_impl} )

INSTALL( FILES ${shape_decl} DESTINATION include/yocto/fovea/shape )
SOURCE_GROUP( fovea/shape FILES ${shape_srcs} )

INCLUDE_DIRECTORIES( . ../main ../mk)

ADD_LIBRARY(y-fovea STATIC 
	${srcs} 
	${mesh_srcs} 
	${shape_srcs} 
)

INSTALL( TARGETS y-fovea ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
