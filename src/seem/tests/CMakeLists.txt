FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-seem main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-seem seem lingua )
