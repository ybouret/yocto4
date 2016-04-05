FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lingua main.cpp ${srcs}
)
TARGET_LINK_YOCTO(test-lingua lingua)
 
ENABLE_TESTING()

ADD_TEST(token test-lingua token)
