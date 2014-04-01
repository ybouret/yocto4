FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-fovea main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fovea mk fovea )

ENABLE_TESTING()

ADD_TEST(layout  test-fovea layout )
ADD_TEST(array1d test-fovea array1d)
ADD_TEST(array2d test-fovea array2d)
ADD_TEST(array3d test-fovea array3d)
ADD_TEST(adb     test-fovea adb    )
ADD_TEST(mesh    test-fovea mesh   )
ADD_TEST(cell    test-fovea cell   )
ADD_TEST(vtk     test-fovea vtk    )
