
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpa main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpa mpa)
