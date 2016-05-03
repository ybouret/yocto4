
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-json main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-json json lingua)

ENABLE_TESTING()
ADD_TEST(Value test-json value)
ADD_TEST(Parser1 test-json parser ${CMAKE_CURRENT_SOURCE_DIR}/sample1.js)
ADD_TEST(Parser2 test-json parser ${CMAKE_CURRENT_SOURCE_DIR}/sample2.js)
ADD_TEST(Parser3 test-json parser ${CMAKE_CURRENT_SOURCE_DIR}/sample3.js)
ADD_TEST(Parser4 test-json parser ${CMAKE_CURRENT_SOURCE_DIR}/sample4.js)
ADD_TEST(ParseDB test-json parser ${CMAKE_CURRENT_SOURCE_DIR}/db.js)
