
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-pack main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-pack pack)

#ENABLE_TESTING()
#ADD_CUSTOM_TARGET(check-pack COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-pack)
