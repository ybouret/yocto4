PROJECT(y-fft)

FILE( GLOB decl yocto/math/fft/*.hpp)
FILE( GLOB impl yocto/math/fft/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION yocto/math/fft )
SOURCE_GROUP( math/fft FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-mk)

ADD_LIBRARY( y-fft STATIC ${srcs} )
INSTALL( TARGETS y-fft ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

