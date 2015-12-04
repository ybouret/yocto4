PROJECT(fltk)

INCLUDE(../share/fltk.cmake)

IF(FLTK_FOUND)

FILE( GLOB decl yocto/FLTK/*.hpp)
FILE( GLOB impl yocto/FLTK/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/FLTK )
SOURCE_GROUP( FLTK FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../mk)

ADD_LIBRARY( y-fltk STATIC ${srcs} )
INSTALL( TARGETS y-fltk ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

ENDIF(FLTK_FOUND)

