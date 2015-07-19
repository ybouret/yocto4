FILE(GLOB test_srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-gfx gfx jpeg png z)

IF(YOCTO_LINUX)
	TARGET_LINK_LIBRARIES(test-gfx dl)
ENDIF()

ENABLE_TESTING()

ADD_TEST(GreyScale test-gfx greyscale)
ADD_TEST(Bitmap    test-gfx bitmap)
ADD_TEST(Pixmap    test-gfx pixmap)

