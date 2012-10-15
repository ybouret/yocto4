
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-mpk main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mpk mpk)

ENABLE_TESTING()
ADD_TEST(Memory  test-mpk mem)
ADD_TEST(Natural test-mpk natural)
ADD_TEST(Primes  test-mpk primes)
ADD_TEST(RSA0    test-mpk rsa0)
ADD_TEST(BitIO   test-mpk bitio)
#ADD_TEST(IO      test-mpk io)

#ADD_CUSTOM_TARGET(check-mpk COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-mpk)
