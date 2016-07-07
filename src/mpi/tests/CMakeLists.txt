
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-mpi main.cpp ${test_srcs})

TARGET_LINK_YOCTO(     test-mpi mpi)
TARGET_LINK_LIBRARIES( test-mpi ${MPI_LIBRARIES})
INSTALL( TARGETS test-mpi RUNTIME DESTINATION share/yocto/demo)


#ENABLE_TESTING()
#GET_TARGET_PROPERTY(PROG test-mpi LOCATION)
#MESSAGE( STATUS "running with ${PROG}")
#ADD_TEST(Init ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 4 ${PROG} init)

#ADD_CUSTOM_TARGET(check-kr COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test-kr )
