
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lingua main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-lingua lingua)

