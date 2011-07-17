
FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-mk main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-mk mk )

ENABLE_TESTING()

ADD_TEST(ZType  test-mk z)
ADD_TEST(Matrix test-mk matrix)
ADD_TEST(LinSys test-mk linsys)
ADD_TEST(Jacobi test-mk jacobi)
ADD_TEST(intg   test-mk intg)
ADD_TEST(drvs   test-mk drvs)
ADD_TEST(drvs   test-mk zero)
ADD_TEST(drvs   test-mk opt)

ADD_CUSTOM_TARGET(check-mk COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-mk )
