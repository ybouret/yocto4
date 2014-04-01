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

FILE( GLOB cell_decl yocto/fovea/cell/*.hpp )
FILE( GLOB cell_impl yocto/fovea/cell/*.cpp )
SET( cell_srcs ${cell_decl} ${cell_impl} )

INSTALL( FILES ${cell_decl} DESTINATION include/yocto/fovea/cell )
SOURCE_GROUP( fovea/cell FILES ${cell_srcs} )

INCLUDE_DIRECTORIES( . ../main ../mk)

ADD_LIBRARY(y-fovea STATIC 
	${srcs} 
	${mesh_srcs} 
	${cell_srcs} 
)

INSTALL( TARGETS y-fovea ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
