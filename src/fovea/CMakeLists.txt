PROJECT(y-fovea)


FILE( GLOB decl yocto/fovea/*.hpp )
FILE( GLOB srcs yocto/fovea/*.cpp )
SET( srcs ${decl} ${srcs} )

INSTALL( FILES ${decl} DESTINATION include/yocto/fovea )
SOURCE_GROUP( fovea FILES ${srcs} )


INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY(y-fovea STATIC ${srcs} )

INSTALL( TARGETS y-fovea ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
