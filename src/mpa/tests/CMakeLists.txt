
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpa main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpa mpa)

ENABLE_TESTING()
ADD_TEST(Mem  test-mpa mem)
ADD_TEST(IO   test-mpa io)
ADD_TEST(Add  test-mpa add)
ADD_TEST(Cmp  test-mpa cmp)
ADD_TEST(Sub  test-mpa sub)
ADD_TEST(Mul  test-mpa mul)
ADD_TEST(Bits test-mpa mul)
