
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-kr main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-kr kr )

ENABLE_TESTING()
ADD_TEST(Hashing test-kr hashing)
ADD_TEST(HMAC    test-kr hmac)
ADD_TEST(SCipher test-kr sc)
ADD_TEST(BCipher test-kr bc)

ADD_CUSTOM_TARGET(check-kr COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test-kr )
