PROJECT(y-pack)

FILE( GLOB decl yocto/pack/*.hpp)
FILE( GLOB impl yocto/pack/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/pack )
SOURCE_GROUP( pack FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-pack STATIC ${srcs} )
INSTALL( TARGETS y-pack ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

