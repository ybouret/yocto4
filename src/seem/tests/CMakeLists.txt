FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-seem main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-seem seem lingua )

ENABLE_TESTING()

ADD_TEST(Eval test-seem eval "1+cos(x)" )