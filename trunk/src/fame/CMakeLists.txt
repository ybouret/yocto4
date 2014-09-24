PROJECT(fame)


FILE( GLOB decl yocto/fame/*.hpp )
FILE( GLOB srcs yocto/fame/*.cpp )
SET( srcs ${decl} ${srcs} )

INSTALL( FILES ${decl} DESTINATION include/yocto/fame )
SOURCE_GROUP( fame FILES ${srcs} )


INCLUDE_DIRECTORIES( . ../main ../mk)

ADD_LIBRARY(y-fame STATIC 
	${srcs} 
)

INSTALL( TARGETS y-fame ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
