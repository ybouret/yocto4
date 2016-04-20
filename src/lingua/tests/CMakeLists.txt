FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lingua main.cpp ${srcs}
	simple.g
	json.g
)
TARGET_LINK_YOCTO(test-lingua lingua)
 
ENABLE_TESTING()

ADD_TEST(token test-lingua token)
