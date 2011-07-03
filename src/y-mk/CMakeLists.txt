PROJECT(y-mk)

FILE( GLOB decl yocto/math/kernel/*.hpp)
FILE( GLOB impl yocto/math/kernel/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION yocto/math/kernel )
SOURCE_GROUP( math/kernel FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-mk STATIC ${srcs} )
INSTALL( TARGETS y-mk ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

