PROJECT(thr)

FILE( GLOB decl yocto/threading/*.hpp)
FILE( GLOB impl yocto/threading/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/threading )
SOURCE_GROUP( threading FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-thr STATIC ${srcs} )
INSTALL( TARGETS y-thr ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

