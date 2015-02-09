
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-gems main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-gems gems )

ENABLE_TESTING()

ADD_TEST(XYZ  test-gems xyz ${CMAKE_CURRENT_SOURCE_DIR}/solid.xyz)


#ADD_CUSTOM_TARGET(check-kr COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test-kr )
