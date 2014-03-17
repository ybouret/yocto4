
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpa main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpa mpa)

ENABLE_TESTING()
ADD_TEST(Mem test-mpa mem)
ADD_TEST(IO  test-mpa io)
