
SET(fx test-lua)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(${fx} main.cpp ${test_srcs})
TARGET_LINK_YOCTO(${fx} lua)
SET( HERE "${CMAKE_SOURCE_DIR}/lua/tests" )

ENABLE_TESTING()
ADD_TEST(Hello   ${fx} "${HERE}/hello.lua" )
ADD_TEST(Bisect  ${fx} "${HERE}/bisect.lua")
ADD_TEST(Sieve   ${fx} "${HERE}/sieve.lua" )
ADD_TEST(Sort    ${fx} "${HERE}/sort.lua"  )

#ADD_CUSTOM_TARGET(check-lua COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-lua)
