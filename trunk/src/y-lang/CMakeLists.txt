PROJECT(y-lang)

FILE( GLOB decl yocto/lang/*.hpp)
FILE( GLOB impl yocto/lang/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lang )
SOURCE_GROUP( lang FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-rx)

ADD_LIBRARY( y-lang STATIC ${srcs} )
INSTALL( TARGETS y-lang ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

