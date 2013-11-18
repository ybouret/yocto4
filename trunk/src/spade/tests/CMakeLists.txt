
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs} main.hpp)
TARGET_LINK_YOCTO( test-spade spade gfx)

ENABLE_TESTING()
ADD_TEST(Coord   test-spade coord)
ADD_TEST(Layout  test-spade layout)
ADD_TEST(Array1D test-spade array1D)
ADD_TEST(Array2D test-spade array2D)
ADD_TEST(Array3D test-spade array3D)
ADD_TEST(Arrays  test-spade arrays)
ADD_TEST(Fields  test-spade fields)
ADD_TEST(Data1D  test-spade data1D)
ADD_TEST(Data2D  test-spade data2D)
ADD_TEST(Data3D  test-spade data3D)
ADD_TEST(Ghosts  test-spade data3D)
ADD_TEST(Mesh    test-spade mesh)
ADD_TEST(Region  test-spade region)
ADD_TEST(Work1D  test-spade work1D)
ADD_TEST(Work2D  test-spade work2D)
ADD_TEST(Work3D  test-spade work3D)
ADD_TEST(IO      test-spade io)
ADD_TEST(Ops     test-spade ops)



