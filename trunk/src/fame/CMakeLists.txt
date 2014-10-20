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

INCLUDE_DIRECTORIES( . ../main ../mk)

ADD_LIBRARY(y-fame STATIC 
	${srcs}
	${mesh_srcs} 
)

INSTALL( TARGETS y-fame ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
