
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-script main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-script script lang)
