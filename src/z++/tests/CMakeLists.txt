
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-z++ main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-z++ z++ z)

