
#common code
FILE( GLOB script_decl yocto/script/*.hpp)
FILE( GLOB script_impl yocto/script/*.cpp)
SET( script_srcs ${script_decl} ${script_impl} )

INSTALL( FILES ${script_decl} DESTINATION include/yocto/script )
SOURCE_GROUP( script FILES ${script_srcs} )

IF(FALSE)
FILE( GLOB mpn_srcs yocto/mpa/natural/*.cpp )
SOURCE_GROUP( mpa/natural FILES ${mpn_srcs} )

FILE( GLOB mpz_srcs yocto/mpa/integer/*.cpp )
SOURCE_GROUP( mpa/integer FILES ${mpz_srcs} )

FILE( GLOB rsa_decl yocto/mpa/rsa/*.hpp)
FILE( GLOB rsa_impl yocto/mpa/rsa/*.cpp)
SET( rsa_srcs ${rsa_decl} ${rsa_impl} )
SOURCE_GROUP( mpa/rsa FILES ${rsa_srcs})
INSTALL( FILES ${rsa_decl} DESTINATION include/yocto/mpa/rsa )
ENDIF()

#gather sources
SET( srcs ${script_srcs} )
INCLUDE_DIRECTORIES( . ../main ../lang)

ADD_LIBRARY( y-script STATIC ${srcs} )
INSTALL( TARGETS y-script ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
