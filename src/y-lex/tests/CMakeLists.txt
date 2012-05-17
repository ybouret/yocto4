
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lex main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-lex lex rx)

#ENABLE_TESTING()
#ADD_TEST(Team      test-thr team)

#ADD_CUSTOM_TARGET(check-thr COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-thr)
