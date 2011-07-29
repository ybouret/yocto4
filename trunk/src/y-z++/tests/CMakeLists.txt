
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-z++ main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-z++ z++ z)

#ENABLE_TESTING()
#ADD_TEST(Posix test-z++ posix)
#ADD_TEST(Compiler test-z++ compiler '[:digit:]+[.]?' no )
#ADD_CUSTOM_TARGET(check-z++ COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-z++)
