PROJECT(random)

FILE( GLOB random_decl yocto/random/*.hpp)
FILE( GLOB random_impl yocto/random/*.cpp)
SET( random_srcs ${random_decl} ${random_impl} )
INSTALL( FILES ${random_decl} DESTINATION include/yocto/random )
SOURCE_GROUP( random FILES ${random_srcs} )

INCLUDE_DIRECTORIES( . ../main )


ADD_LIBRARY( y-random STATIC ${random_srcs}  )
INSTALL( TARGETS y-random ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

