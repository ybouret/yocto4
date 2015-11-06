
FILE( GLOB decl yocto/chemical/*.hpp )
FILE( GLOB impl yocto/chemical/*.cpp )
SET(srcs ${decl} ${impl})
SOURCE_GROUP(chemical FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../mk ../third_party/lua/yocto/lua ../lua++ )

ADD_LIBRARY(y-chemical STATIC ${srcs} )
ADD_SUBDIRECTORY(tests)
