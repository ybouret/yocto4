PROJECT(y-spade)

FILE( GLOB decl yocto/spade/*.hpp)
FILE( GLOB impl yocto/spade/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/spade )
SOURCE_GROUP( spade FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-spade STATIC ${srcs} )
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

IF(FALSE)

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

ENDIF(FALSE)

