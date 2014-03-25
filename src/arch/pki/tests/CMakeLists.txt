
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-pki main.cpp ${test_srcs} rsa-keys.h two-keys.h)
TARGET_LINK_YOCTO(test-pki pki mpk)

ENABLE_TESTING()
ADD_TEST(RSA1   test-pki rsa1)
ADD_TEST(Actors test-pki actors session_key)

#ADD_CUSTOM_TARGET(check-pki COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-pki)
