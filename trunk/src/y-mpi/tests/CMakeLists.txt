
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-mpi main.cpp ${test_srcs})

TARGET_LINK_YOCTO(     test-mpi mpi )
TARGET_LINK_LIBRARIES( test-mpi ${MPI_LIBRARIES})

#ENABLE_TESTING()
#ADD_CUSTOM_TARGET(check-kr COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test-kr )
