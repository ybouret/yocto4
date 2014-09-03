FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang main.cpp ${srcs})
TARGET_LINK_YOCTO( test-lang lang )

ENABLE_TESTING()

ADD_TEST(Token  test-lang token)
ADD_TEST(Dict   test-lang dict)
ADD_TEST(Viz    test-lang viz)
