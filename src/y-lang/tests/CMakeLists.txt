
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lang main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-lang lang)


ENABLE_TESTING()
ADD_TEST(RXPosix    test-lang posix)
ADD_TEST(RXCompiler test-lang comp_rx '[:digit:]+[.]?' no )

#ADD_CUSTOM_TARGET(check-lang COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-lang)
