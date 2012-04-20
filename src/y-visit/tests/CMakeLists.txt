
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-visit main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-visit visit)
TARGET_LINK_LIBRARIES( test-visit simV2 simV2f)

#ENABLE_TESTING()
#ADD_TEST(Condition test-visit condition)
#ADD_TEST(Barrier   test-visit barrier)
#ADD_TEST(Team      test-visit team)

#ADD_CUSTOM_TARGET(check-thr COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-thr)
