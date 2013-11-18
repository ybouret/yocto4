
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-random main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-random random)

ENABLE_TESTING()
ADD_TEST(Dist test-random dist)
#ADD_CUSTOM_TARGET(check-random COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-random)
