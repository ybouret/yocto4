PROJECT(y-mpi)

FIND_PACKAGE(MPI)

IF(MPI_FOUND)

  MESSAGE( STATUS "[configuring y-mpi]" )
  
  FILE( GLOB decl yocto/mpi/*.hpp )
  FILE( GLOB impl yocto/mpi/*.cpp )
  SET( srcs ${decl} ${impl} )

  INCLUDE_DIRECTORIES( . ../y-main )
  INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})


  ADD_LIBRARY(y-mpi STATIC ${srcs} )
  
  ADD_SUBDIRECTORY(tests)
  
ENDIF()
