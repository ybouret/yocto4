FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-fovea main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fovea fovea )
