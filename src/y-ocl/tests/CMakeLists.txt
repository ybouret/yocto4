
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-ocl main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-ocl ocl )
OPENCL_LINK_TO(test-ocl)

ENABLE_TESTING()
ADD_TEST(Platform test-ocl platform)
ADD_TEST(Context  test-ocl context)
ADD_TEST(Buffer   test-ocl buffer)
ADD_TEST(Images   test-ocl images)
ADD_TEST(Kernel   test-ocl kernel)

ADD_CUSTOM_TARGET(check-ocl COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-ocl )
