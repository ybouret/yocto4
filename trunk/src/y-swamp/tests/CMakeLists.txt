
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-swamp main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-swamp swamp)

#ENABLE_TESTING()
#ADD_TEST(Condition test-thr condition)
#ADD_TEST(Barrier   test-thr barrier)
#ADD_TEST(Team      test-thr team)

#ADD_CUSTOM_TARGET(check-thr COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-thr)
