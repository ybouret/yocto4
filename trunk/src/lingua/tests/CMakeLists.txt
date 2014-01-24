
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lingua main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-lingua lingua)

ENABLE_TESTING()

ADD_TEST(Dict   test-lingua dict)
ADD_TEST(Viz    test-lingua viz)
ADD_TEST(Token  test-lingua token)

