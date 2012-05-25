
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-swamp-ocl main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-swamp-ocl swamp ocl)
OPENCL_LINK_TO(test-swamp-ocl)
