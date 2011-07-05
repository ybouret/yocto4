
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-fft main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fft fft mk)

ENABLE_TESTING()
ADD_TEST(FFT1 test-fft fft1)
ADD_TEST(PSD  test-fft psd 10 8)



ADD_CUSTOM_TARGET(check-fft COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-fft)
