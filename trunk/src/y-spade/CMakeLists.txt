PROJECT(y-spade)

FILE( GLOB decl yocto/spade/*.hpp)
FILE( GLOB impl yocto/spade/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/spade )
SOURCE_GROUP( spade FILES ${srcs} )

FILE( GLOB fields_decl yocto/spade/fields/*.hpp )
FILE( GLOB fields_impl yocto/spade/fields/*.cpp )
SET( fields_srcs ${fields_decl} ${fields_impl}  )
INSTALL( FILES ${fields_decl} DESTINATION include/yocto/spade/fields )
SOURCE_GROUP( spade/fields FILES ${fields_srcs} )

FILE( GLOB ghosts_decl yocto/spade/ghosts/*.hpp )
FILE( GLOB ghosts_impl yocto/spade/ghosts/*.cpp )
SET( ghosts_srcs ${ghosts_decl} ${ghosts_impl}  )
INSTALL( FILES ${ghosts_decl} DESTINATION include/yocto/spade/ghosts )
SOURCE_GROUP( spade/ghosts FILES ${ghosts_srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-spade STATIC ${srcs} ${fields_srcs} ${ghosts_srcs} )
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)


FIND_PACKAGE(MPI)

IF(MPI_FOUND)
	INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH} ../y-mpi)
	ADD_SUBDIRECTORY(tests-mpi)
ENDIF(MPI_FOUND)

IF(FALSE)

INCLUDE(../share/opencl.cmake)

IF(OPENCL_FOUND)
	INCLUDE_DIRECTORIES(../y-ocl)
	ADD_SUBDIRECTORY(tests-ocl)
ENDIF(OPENCL_FOUND)

ENDIF(FALSE)

