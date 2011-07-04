
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-mk main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mk mk )

ENABLE_TESTING()
ADD_TEST(ZType test-mk z)
ADD_TEST(Matrix text-mk matrix)


ADD_CUSTOM_TARGET(check-mk COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-mk )
