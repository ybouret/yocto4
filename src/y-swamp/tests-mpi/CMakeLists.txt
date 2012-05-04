
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-swamp-mpi main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-swamp-mpi swamp mpi)
TARGET_LINK_LIBRARIES( test-swamp-mpi ${MPI_LIBRARIES})
