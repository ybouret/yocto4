PROJECT(kr)

FILE( GLOB hashing_decl yocto/hashing/*.hpp)
FILE( GLOB hashing_impl yocto/hashing/*.cpp)
SET( hashing_srcs ${hashing_decl} ${hashing_impl} )
INSTALL( FILES ${hashing_decl} DESTINATION include/yocto/hashing )
SOURCE_GROUP( hashing FILES ${hashing_srcs} )

FILE( GLOB bc_decl yocto/crypto/bc/*.hpp)
FILE( GLOB bc_impl yocto/crypto/bc/*.cpp)
SET( bc_srcs ${bc_decl} ${bc_impl} )
INSTALL( FILES ${bc_decl} DESTINATION include/yocto/crypto/bc )
SOURCE_GROUP( crypto/bc FILES ${bc_srcs} )

FILE( GLOB sc_decl yocto/crypto/sc/*.hpp)
FILE( GLOB sc_impl yocto/crypto/sc/*.cpp)
SET( sc_srcs ${sc_decl} ${sc_impl} )
INSTALL( FILES ${sc_decl} DESTINATION include/yocto/crypto/sc )
SOURCE_GROUP( crypto/sc FILES ${sc_srcs} )


FILE( GLOB ios_decl yocto/crypto/ios/*.hpp)
FILE( GLOB ios_impl yocto/crypto/ios/*.cpp)
SET( ios_srcs ${ios_decl} ${ios_impl} )
INSTALL( FILES ${ios_decl} DESTINATION include/yocto/crypto/ios )
SOURCE_GROUP( crypto/ios FILES ${ios_srcs} )


SET( decl ${hashing_decl} ${bc_decl} ${sc_decl} ${ios_decl})
SET( srcs ${hashing_srcs} ${bc_srcs} ${sc_srcs} ${ios_srcs})

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY(y-kr STATIC ${srcs} )

INSTALL( TARGETS y-kr ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

ADD_EXECUTABLE(y-md y-md.cpp)
TARGET_LINK_YOCTO(y-md kr)
INSTALL( TARGETS y-md RUNTIME DESTINATION bin )

