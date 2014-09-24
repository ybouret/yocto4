FILE(GLOB test_srcs test-*.cpp *.hpp)
ADD_EXECUTABLE(test-fame main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-fame mk fame )

ENABLE_TESTING()

ADD_TEST(Layout test-fame layout)