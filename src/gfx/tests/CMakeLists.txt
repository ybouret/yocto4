FILE(GLOB test_srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-gfx gfx png tiff)

ENABLE_TESTING()
ADD_TEST(Types test-gfx types)
ADD_TEST(PixMap test-gfx pixmap)

