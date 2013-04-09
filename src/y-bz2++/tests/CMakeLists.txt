
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-bz2++ main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-bz2++ bz2++ bz2)

