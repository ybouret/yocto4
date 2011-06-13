
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-kr main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-kr kr )

ENABLE_TESTING()
ADD_TEST(Hashing test-kr hashing)

ADD_CUSTOM_TARGET(check-kr COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-kr )
