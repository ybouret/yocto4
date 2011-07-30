
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-spade spade)

ENABLE_TESTING()
ADD_TEST(Layouts test-spade layouts)
ADD_TEST(a1D test-spade a1D)
ADD_TEST(a2D test-spade a2D)
ADD_TEST(a3D test-spade a3D)
ADD_TEST(s1D test-spade s1D)
ADD_TEST(s2D test-spade s2D)
ADD_TEST(w1D test-spade w1D)
ADD_TEST(w2D test-spade w2D)
ADD_TEST(w3D test-spade w3D)
ADD_TEST(ppm test-spade ppm)
ADD_TEST(bresenham test-spade bresenham)

ADD_CUSTOM_TARGET(check-spade COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-spade)
