FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-fame main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fame mk fame )

ENABLE_TESTING()

ADD_TEST(Layout  test-fame layout)
ADD_TEST(Array1D test-fame array1d)
ADD_TEST(Array2D test-fame array2d)
ADD_TEST(Array3D test-fame array3d)
ADD_TEST(Arrays  test-fame arrays)

FIND_PACKAGE(MPI)

IF(MPI_FOUND)
INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
INCLUDE_DIRECTORIES(../../mpi)

ADD_EXECUTABLE(test-fame-par main-par.cpp)
TARGET_LINK_YOCTO(test-fame-par mpa mpi)
TARGET_LINK_LIBRARIES(test-fame-par ${MPI_LIBRARIES})


ENDIF(MPI_FOUND)
