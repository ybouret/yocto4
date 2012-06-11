PROJECT(y-fltk)

INCLUDE(../share/fltk.cmake)

IF(FLTK_FOUND)

FILE( GLOB decl yocto/FL/*.hpp)
FILE( GLOB impl yocto/FL/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/FL )
SOURCE_GROUP( FL FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-fltk STATIC ${srcs} )
INSTALL( TARGETS y-fltk ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

ENDIF(FLTK_FOUND)

