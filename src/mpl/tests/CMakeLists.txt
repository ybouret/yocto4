
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpl main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpl mpl)

ENABLE_TESTING()
ADD_TEST(mem test-mpl mem)
ADD_TEST(mpn test-mpl mpn)

FIND_PACKAGE(MPI)

IF(MPI_FOUND)
INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
INCLUDE_DIRECTORIES(../../mpi)

ADD_EXECUTABLE(test-mpl-par main-par.cpp)
TARGET_LINK_YOCTO(test-mpl-par mpl mpi)
TARGET_LINK_LIBRARIES(test-mpl-par ${MPI_LIBRARIES})



ENDIF(MPI_FOUND)

