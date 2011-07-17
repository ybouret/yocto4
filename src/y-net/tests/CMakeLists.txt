
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-net main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-net net)

ENABLE_TESTING()

ADD_CUSTOM_TARGET(check-net COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-net)
