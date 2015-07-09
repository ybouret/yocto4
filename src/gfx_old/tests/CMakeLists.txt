
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-gfx_old main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-gfx_old gfx_old png z jpeg)

IF(YOCTO_LINUX)
        TARGET_LINK_LIBRARIES(test-gfx_old dl)
ENDIF()

#ENABLE_TESTING()

#ADD_TEST(Bitmap test-gfx bitmap)
#ADD_TEST(Pixmap test-gfx pixmap)
#ADD_TEST(Rawpix test-gfx rawpix)




