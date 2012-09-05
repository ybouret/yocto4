
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-spade spade )

#ENABLE_TESTING()

#ADD_CUSTOM_TARGET(check-swamp COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-swamp)
