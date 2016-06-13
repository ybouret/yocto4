FILE(GLOB test_srcs test-*.cpp)

ADD_EXECUTABLE(test-graphics main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-graphics graphics png tiff)

ENABLE_TESTING()

ADD_TEST(Bitmap test-graphics bitmap)
ADD_TEST(Pixmap test-graphics pixmap)
ADD_TEST(Types  test-graphics types)

