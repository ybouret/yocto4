PROJECT(y-spade)

FILE( GLOB spade_decl yocto/spade/*.hpp)
FILE( GLOB spade_impl yocto/spade/*.cpp)
SET( spade_srcs ${spade_decl} ${spade_impl} )
INSTALL( FILES ${spade_decl} DESTINATION include/yocto/spade )
SOURCE_GROUP( spade FILES ${spade_srcs} )


INCLUDE_DIRECTORIES( . ../y-main )


ADD_LIBRARY( y-spade STATIC ${spade_srcs} ${pat_srcs} )
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

#ADD_SUBDIRECTORY(tests)

