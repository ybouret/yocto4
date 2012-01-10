
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-cliff main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-cliff cliff)

ENABLE_TESTING()

ADD_TEST(Layout    test-cliff layout  )
ADD_TEST(Array     test-cliff array   )
ADD_TEST(Split     test-cliff split   )
ADD_TEST(PPM       test-cliff ppm     )
ADD_TEST(Region    test-cliff region  )
ADD_TEST(Workspace test-cliff wksp    )
ADD_TEST(Contour2D test-cliff contour2)
ADD_TEST(Contour3D test-cliff contour3)

ADD_CUSTOM_TARGET(check-cliff COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-cliff)
