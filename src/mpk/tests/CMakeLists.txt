
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpk main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpk mpk)

