
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpk main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpk mpk)

ENABLE_TESTING()
ADD_TEST(types test-mpk types)
ADD_TEST(mpn   test-mpk mpn)
ADD_TEST(mul   test-mpk mul)
