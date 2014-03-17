PROJECT(y-mpa)

#common code
FILE( GLOB mpa_decl yocto/mpa/*.hpp)
FILE( GLOB mpa_impl yocto/mpa/*.cpp)
SET( mpa_srcs ${mpa_decl} ${mpa_impl} )

INSTALL( FILES ${mpa_decl} DESTINATION include/yocto/mpa )
SOURCE_GROUP( mpa FILES ${mpa_srcs} )


#gather sources
SET( srcs ${mpa_srcs} )
INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-mpa STATIC ${srcs} )
INSTALL( TARGETS y-mpa ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
