PROJECT(y-kr)
FILE( GLOB hashing_decl yocto/hashing/*.hpp)
FILE( GLOB hashing_impl yocto/hashing/*.cpp)
SET( hashing_srcs ${hashing_decl} ${hashing_impl} )
INSTALL( FILES ${hashing_decl} DESTINATION include/yocto/hashing )
SOURCE_GROUP( hashing FILES ${hashing_srcs} )

SET( decl ${hashing_decl})
SET( srcs ${hashing_srcs})

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY(y-kr STATIC ${srcs} )

INSTALL( TARGETS y-kr ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)