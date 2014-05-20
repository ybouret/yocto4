
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpa main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpa mpa)

ENABLE_TESTING()
ADD_TEST(Mem   test-mpa mem)
ADD_TEST(IO    test-mpa io)
ADD_TEST(Add   test-mpa add)
ADD_TEST(Cmp   test-mpa cmp)
ADD_TEST(Sub   test-mpa sub)
ADD_TEST(Mul   test-mpa mul)
ADD_TEST(Bits  test-mpa bits)
ADD_TEST(Div   test-mpa div)
ADD_TEST(Ari   test-mpa ari)
ADD_TEST(Prime test-mpa prime)
ADD_TEST(RSA   test-mpa rsa)
ADD_TEST(Keys  test-mpa rsa_keys)
ADD_TEST(Pow   test-mpa pow)
ADD_TEST(Int   test-mpa int)
ADD_TEST(Rational test-mpa rational)
ADD_TEST(Split   test-mpa  split)

FIND_PACKAGE(MPI)

IF(MPI_FOUND)
INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
INCLUDE_DIRECTORIES(../../mpi)

ADD_EXECUTABLE(test-mpa-par main-par.cpp)
TARGET_LINK_YOCTO(test-mpa-par mpa mpi)
TARGET_LINK_LIBRARIES(test-mpa-par ${MPI_LIBRARIES})



ENDIF(MPI_FOUND)


