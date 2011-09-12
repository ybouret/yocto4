
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-ocl main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-ocl ocl )
OPENCL_LINK_TO(test-ocl)

#ENABLE_TESTING()
#ADD_TEST(Hashing test-ocl hashing)
#ADD_TEST(HMAC    test-ocl hmac)
#ADD_TEST(SCipher test-ocl sc)
#ADD_TEST(BCipher test-ocl bc)

#ADD_CUSTOM_TARGET(check-ocl COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test-ocl )
