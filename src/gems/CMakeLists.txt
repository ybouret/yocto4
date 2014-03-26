PROJECT(gems)

FILE( GLOB decl yocto/gems/*.hpp)
FILE( GLOB impl yocto/gems/*.cpp)
SET(srcs ${decl} ${impl})
INSTALL( FILES ${decl} DESTINATION include/yocto/gems )
SOURCE_GROUP( gems FILES ${srcs} )

FILE( GLOB io_decl yocto/gems/io/*.hpp)
FILE( GLOB io_impl yocto/gems/io/*.cpp)
SET( io_srcs ${io_decl} ${io_impl} )
INSTALL( FILES ${io_decl} DESTINATION include/yocto/gems/io )
SOURCE_GROUP( gems/io FILES ${io_srcs} )

SET( gems_srcs ${srcs} ${io_srcs} )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY(y-gems STATIC ${gems_srcs} )

INSTALL( TARGETS y-gems ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
