
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-swamp main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-swamp swamp)

ENABLE_TESTING()
ADD_TEST(Layout  test-swamp layout)
ADD_TEST(Array   test-swamp array)
ADD_TEST(Arrays  test-swamp arrays)
ADD_TEST(RMesh   test-swamp rmesh)
ADD_TEST(CMesh   test-swamp cmesh)
ADD_TEST(Data    test-swamp data)
ADD_TEST(Wksp    test-swamp wksp)
ADD_TEST(Fill    test-swamp fill)

ADD_CUSTOM_TARGET(check-swamp COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-swamp)
