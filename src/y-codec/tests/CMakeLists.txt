
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-codec main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-codec codec)

#ENABLE_TESTING()


#ADD_CUSTOM_TARGET(check-thr COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-thr)
