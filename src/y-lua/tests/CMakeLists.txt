
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lua main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-lua lua)

#ENABLE_TESTING()

#ADD_CUSTOM_TARGET(check-lua COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-lua)
