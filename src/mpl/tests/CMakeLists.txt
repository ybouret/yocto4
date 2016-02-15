
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpl main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpl mpl)

ENABLE_TESTING()
ADD_TEST(mem test-mpk mem)
