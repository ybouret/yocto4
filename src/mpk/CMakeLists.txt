PROJECT(mpk)

#common code
FILE( GLOB mpk_decl yocto/mpk/*.hpp)
FILE( GLOB mpk_impl yocto/mpk/*.cpp)
SET( mpk_srcs ${mpk_decl} ${mpk_impl} )

INSTALL( FILES ${mpk_decl} DESTINATION include/yocto/mpk )
SOURCE_GROUP( mpk FILES ${mpk_srcs} )

FILE( GLOB mpn_srcs yocto/mpk/natural/*.cpp )
SOURCE_GROUP( mpk/natural FILES ${mpn_srcs} )

FILE( GLOB mpz_srcs yocto/mpk/integer/*.cpp )
SOURCE_GROUP( mpk/integer FILES ${mpz_srcs} )

#FILE( GLOB rsa_decl yocto/mpk/rsa/*.hpp)
#FILE( GLOB rsa_impl yocto/mpk/rsa/*.cpp)
#SET( rsa_srcs ${rsa_decl} ${rsa_impl} )
#SOURCE_GROUP( mpk/rsa FILES ${rsa_srcs})
#INSTALL( FILES ${rsa_decl} DESTINATION include/yocto/mpk/rsa )

#gather sources
SET( srcs ${mpk_srcs} ${mpn_srcs} ${mpz_srcs} ${rsa_srcs} )
INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-mpk STATIC ${srcs} )
INSTALL( TARGETS y-mpk ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
