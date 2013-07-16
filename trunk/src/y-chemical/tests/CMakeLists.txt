INCLUDE_DIRECTORIES(../../y-spade)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-chemical main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-chemical chemical mk lua++ lua)

ENABLE_TESTING()
ADD_TEST(Species test-chemical species)

