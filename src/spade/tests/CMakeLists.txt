FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-spade mk graphics spade png z )
