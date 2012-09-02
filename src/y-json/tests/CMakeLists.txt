
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-json main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-json json lang)

ENABLE_TESTING()
ADD_TEST(JSON_Value test-json value)

#ADD_CUSTOM_TARGET(check-json COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-json)
