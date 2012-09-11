
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade-mpi main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-spade-mpi spade mpi)
TARGET_LINK_LIBRARIES( test-spade-mpi ${MPI_LIBRARIES})
