PROJECT(y-csv)

FILE( GLOB decl yocto/csv/*.hpp)
FILE( GLOB impl yocto/csv/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/csv )
SOURCE_GROUP( csv FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-lang)

ADD_LIBRARY( y-csv STATIC ${srcs} )
INSTALL( TARGETS y-csv ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

