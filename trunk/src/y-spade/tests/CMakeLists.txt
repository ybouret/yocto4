
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs} main.hpp)
TARGET_LINK_YOCTO( test-spade spade )

ENABLE_TESTING()
ADD_TEST(Coord   test-spade coord)
ADD_TEST(Layout  test-spade layout)
ADD_TEST(Array1D test-spade array1D)
ADD_TEST(Array2D test-spade array2D)
ADD_TEST(Array3D test-spade array3D)
ADD_TEST(Arrays  test-spade arrays)
