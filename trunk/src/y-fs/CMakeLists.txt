PROJECT(y-fs)

FILE( GLOB decl yocto/filesys/*.hpp)
FILE( GLOB impl yocto/filesys/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/filesys )
SOURCE_GROUP( filesys FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-fs STATIC ${srcs} )
INSTALL( TARGETS y-fs ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

