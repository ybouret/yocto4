
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-csv main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-csv csv lang)

#ENABLE_TESTING()

#ADD_CUSTOM_TARGET(check-json COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-json)
