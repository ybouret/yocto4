FILE(GLOB test_srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-gfx gfx)

ENABLE_TESTING()

ADD_TEST(GreyScale test-gfx greyscale)
