
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs} main.hpp)
TARGET_LINK_YOCTO( test-spade spade )

ENABLE_TESTING()
ADD_TEST(Coord   test-spade coord)
ADD_TEST(Layout  test-spade layout)
ADD_TEST(Array1D test-spade array1D)

#ADD_CUSTOM_TARGET(check-swamp COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-swamp)
