
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-fs main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fs fs)

ENABLE_TESTING()
ADD_TEST(Scan test-fs scan)
ADD_TEST(WD   test-fs wd)

ADD_CUSTOM_TARGET(check-fs COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-fs)
