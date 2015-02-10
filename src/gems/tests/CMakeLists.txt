
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-gems main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-gems gems )


ENABLE_TESTING()

ADD_TEST(XYZ  test-gems xyz ${CMAKE_CURRENT_SOURCE_DIR}/solid.xyz)


#ADD_CUSTOM_TARGET(check-kr COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test-kr )

FIND_PACKAGE(MPI)

IF(MPI_FOUND)
INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
INCLUDE_DIRECTORIES(../../mpi)

ADD_EXECUTABLE(test-gems-par main-par.cpp)
TARGET_LINK_YOCTO(test-gems-par gems mpi)
TARGET_LINK_LIBRARIES(test-gems-par ${MPI_LIBRARIES})



ENDIF(MPI_FOUND)

