PROJECT(mpa)

#common code
FILE( GLOB mpa_decl yocto/mpa/*.hpp)
FILE( GLOB mpa_impl yocto/mpa/*.cpp)
SET( mpa_srcs ${mpa_decl} ${mpa_impl} )

INSTALL( FILES ${mpa_decl} DESTINATION include/yocto/mpa )
SOURCE_GROUP( mpa FILES ${mpa_srcs} )

FILE( GLOB mpn_srcs yocto/mpa/natural/*.cpp )
SOURCE_GROUP( mpa/natural FILES ${mpn_srcs} )

FILE( GLOB mpz_srcs yocto/mpa/integer/*.cpp )
SOURCE_GROUP( mpa/integer FILES ${mpz_srcs} )

FILE( GLOB rsa_decl yocto/mpa/rsa/*.hpp)
FILE( GLOB rsa_impl yocto/mpa/rsa/*.cpp)
SET( rsa_srcs ${rsa_decl} ${rsa_impl} )
SOURCE_GROUP( mpa/rsa FILES ${rsa_srcs})
INSTALL( FILES ${rsa_decl} DESTINATION include/yocto/mpa/rsa )

#gather sources
SET( srcs ${mpa_srcs} ${mpn_srcs} ${mpz_srcs} ${rsa_srcs} )
INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-mpa STATIC ${srcs} )
INSTALL( TARGETS y-mpa ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
