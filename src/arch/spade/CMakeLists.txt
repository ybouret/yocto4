PROJECT(spade)

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

FILE( GLOB vtk_decl yocto/spade/vtk/*.hpp )
FILE( GLOB vtk_impl yocto/spade/vtk/*.cpp )
SET( vtk_srcs ${vtk_decl} ${vtk_impl}  )
INSTALL( FILES ${vtk_decl} DESTINATION include/yocto/spade/vtk )
SOURCE_GROUP( spade/vtk FILES ${vtk_srcs} )

FILE( GLOB mpi_decl yocto/spade/mpi/*.hpp )
SET( mpi_srcs ${mpi_decl} )
INSTALL( FILES ${mpi_decl} DESTINATION include/yocto/spade/mpi )
SOURCE_GROUP( spade/mpi FILES ${mpi_srcs} )

FILE( GLOB contour_decl yocto/spade/contour/*.hpp )
FILE( GLOB contour_impl yocto/spade/contour/*.cpp )
SET( contour_srcs ${contour_decl} ${contour_impl}  )
INSTALL( FILES ${contour_decl} DESTINATION include/yocto/spade/contour )
SOURCE_GROUP( spade/contour FILES ${contour_srcs} )

FILE( GLOB ops_decl yocto/spade/ops/*.hpp )
FILE( GLOB ops_impl yocto/spade/ops/*.cpp )
SET( ops_srcs ${ops_decl} ${ops_impl})
INSTALL( FILES ${ops_decl} DESTINATION include/yocto/spade/ops ) 
SOURCE_GROUP( spade/ops FILES ${ops_srcs} )

INCLUDE_DIRECTORIES( . ../main ../gfx)

ADD_LIBRARY( y-spade STATIC ${srcs} ${fields_srcs} ${ghosts_srcs} ${vtk_srcs} ${mpi_srcs} ${contour_srcs} ${ops_srcs})
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)


FIND_PACKAGE(MPI)

IF(MPI_FOUND)
	INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH} ../mpi)
	ADD_SUBDIRECTORY(tests-mpi)
ENDIF(MPI_FOUND)

IF(FALSE)

INCLUDE(../share/opencl.cmake)

IF(OPENCL_FOUND)
	INCLUDE_DIRECTORIES(../ocl)
	ADD_SUBDIRECTORY(tests-ocl)
ENDIF(OPENCL_FOUND)

ENDIF(FALSE)

