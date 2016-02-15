#common code
FILE( GLOB mpl_decl yocto/mpl/*.hpp)
FILE( GLOB mpl_impl yocto/mpl/*.cpp)
SET( mpl_srcs ${mpl_decl} ${mpl_impl} )

INSTALL( FILES ${mpl_decl} DESTINATION include/yocto/mpl )
SOURCE_GROUP( mpl FILES ${mpl_srcs} )

FILE( GLOB mpn_srcs yocto/mpl/natural/*.cpp )
SOURCE_GROUP( mpl/natural FILES ${mpn_srcs} )

SET( srcs ${mpl_srcs} ${mpn_srcs} ${mpz_srcs} ${rsa_srcs} )
INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-mpl STATIC ${srcs} )
INSTALL( TARGETS y-mpl ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
