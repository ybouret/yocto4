
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lua++ main.cpp ${test_srcs} vec3.lua array.lua tables.lua)
TARGET_LINK_YOCTO( test-lua++ lua++ lua )
SET( HERE "${CMAKE_SOURCE_DIR}/lua++/tests" )

ENABLE_TESTING()
ADD_TEST(vec3    test-lua++ require "${HERE}/vec3.lua" )
ADD_TEST(array   test-lua++ require "${HERE}/array.lua")

#ADD_CUSTOM_TARGET(check-lua++ COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-lua++)
