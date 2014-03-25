PROJECT(y-aqueous)

FILE( GLOB decl yocto/aqueous/*.hpp)
FILE( GLOB impl yocto/aqueous/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/aqueous )
SOURCE_GROUP( aqueous FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../mk ../lua ../lua++ )

ADD_LIBRARY( y-aqueous STATIC ${srcs} )
INSTALL( TARGETS y-aqueous ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

