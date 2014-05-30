
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-gfx main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-gfx gfx jpeg)

ENABLE_TESTING()

ADD_TEST(Bitmap  test-gfx bitmap)
ADD_TEST(Pixmap  test-gfx pixmap)
ADD_TEST(RGB     test-gfx rgb)
ADD_TEST(Pixfmt  test-gfx pixfmt)
ADD_TEST(Metrics test-gfx metrics)
ADD_TEST(Surface test-gfx surface)
ADD_TEST(Save    test-gfx save)
ADD_TEST(Image   test-gfx image)
ADD_TEST(Ramp    test-gfx ramp)




