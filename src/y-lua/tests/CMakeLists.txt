
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lua main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-lua lua)
SET( HERE "${CMAKE_SOURCE_DIR}/y-lua/tests" )
#MESSAGE( STATUS "for lua: CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}" )

ENABLE_TESTING()
ADD_TEST(Hello   test-lua "${HERE}/hello.lua" )
ADD_TEST(Bisect  test-lua "${HERE}/bisect.lua")
ADD_TEST(Sieve   test-lua "${HERE}/sieve.lua" )
ADD_TEST(Sort    test-lua "${HERE}/sort.lua"  )

ADD_CUSTOM_TARGET(check-lua COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-lua)
