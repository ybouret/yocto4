
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-main main.cpp ${test_srcs})
TARGET_LINK_LIBRARIES( test-main yocto )

ENABLE_TESTING()
ADD_TEST(Platform test-main platform)
ADD_TEST(Mutex    test-main mutex)
ADD_TEST(Round    test-main round)
ADD_TEST(Unroll   test-main unroll)
ADD_TEST(WTime    test-main wtime)

ADD_CUSTOM_TARGET(check-main COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-main)
