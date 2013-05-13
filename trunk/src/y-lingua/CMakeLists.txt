PROJECT(y-lang)

FILE( GLOB decl yocto/lingua/*.hpp)
FILE( GLOB impl yocto/lingua/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lingua )
SOURCE_GROUP( lingua FILES ${srcs} )


INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-lingua STATIC ${srcs} )
INSTALL( TARGETS y-lingua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

