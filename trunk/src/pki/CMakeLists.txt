PROJECT(y-pki)

FILE( GLOB decl yocto/pki/*.hpp)
FILE( GLOB impl yocto/pki/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/pki )
SOURCE_GROUP( pki FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../mpk)

ADD_LIBRARY( y-pki STATIC ${srcs} )
INSTALL( TARGETS y-pki ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

