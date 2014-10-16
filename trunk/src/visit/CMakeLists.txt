PROJECT(visit)

INCLUDE(../share/visit.cmake)

IF(VISIT_FOUND)

FILE( GLOB decl yocto/visit/*.hpp)
FILE( GLOB impl yocto/visit/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/visit )
SOURCE_GROUP( visit FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../mpi)

ADD_LIBRARY( y-visit STATIC ${srcs} )
INSTALL( TARGETS y-visit ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)


ENDIF(VISIT_FOUND)

#ADD_SUBDIRECTORY(pjacobi)

