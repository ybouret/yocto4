PROJECT(y-chemical)

FILE( GLOB decl yocto/chemical/*.hpp)
FILE( GLOB impl yocto/chemical/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/chemical )
SOURCE_GROUP( chemical FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-mk ../y-lua ../y-lua++ )

ADD_LIBRARY( y-chemical STATIC ${srcs} )
INSTALL( TARGETS y-chemical ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

