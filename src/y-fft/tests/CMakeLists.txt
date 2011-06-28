
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-fft main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fft fft )

ENABLE_TESTING()



ADD_CUSTOM_TARGET(check-fft COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-fft)
