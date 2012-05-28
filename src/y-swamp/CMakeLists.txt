PROJECT(y-swamp)

FILE( GLOB decl yocto/swamp/*.hpp)
FILE( GLOB impl yocto/swamp/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/swamp )
SOURCE_GROUP( swamp FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-swamp STATIC ${srcs} )
INSTALL( TARGETS y-swamp ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

FIND_PACKAGE(MPI)

IF(MPI_FOUND)
	INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH} ../y-mpi)
	ADD_SUBDIRECTORY(tests-mpi)
ENDIF(MPI_FOUND)

INCLUDE(../share/opencl.cmake)

IF(OPENCL_FOUND)
	INCLUDE_DIRECTORIES(../y-ocl)
	ADD_SUBDIRECTORY(tests-ocl)
ENDIF(OPENCL_FOUND)

