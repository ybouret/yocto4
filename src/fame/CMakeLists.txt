PROJECT(fame)


FILE( GLOB decl yocto/fame/*.hpp )
FILE( GLOB impl yocto/fame/*.cpp )
SET( srcs ${decl} ${impl} )


INSTALL( FILES ${decl} DESTINATION include/yocto/fame )
SOURCE_GROUP( fame FILES ${srcs} )

FILE( GLOB mesh_decl yocto/fame/mesh/*.hpp )
FILE( GLOB mesh_impl yocto/fame/mesh/*.cpp )
SET( mesh_srcs ${mesh_decl} ${mesh_impl} )

INSTALL( FILES ${mesh_decl} DESTINATION include/yocto/fame/mesh )
SOURCE_GROUP( fame/mesh FILES ${mesh_srcs} )

FILE( GLOB ghost_decl yocto/fame/ghost/*.hpp )
FILE( GLOB ghost_impl yocto/fame/ghost/*.cpp )
SET( ghost_srcs ${ghost_decl} ${ghost_impl} )

INSTALL( FILES ${ghost_decl} DESTINATION include/yocto/fame/ghost )
SOURCE_GROUP( fame/ghost FILES ${ghost_srcs} )


INCLUDE_DIRECTORIES( . ../main ../mk)

ADD_LIBRARY(y-fame STATIC 
	${srcs}
	${mesh_srcs} 
	${ghost_srcs}
)

INSTALL( TARGETS y-fame ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
