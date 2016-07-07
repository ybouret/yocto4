FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lingua main.cpp ${srcs}
simple.g
json.g
lists.g
eval.g
calc.g
)
TARGET_LINK_YOCTO(test-lingua lingua)
INSTALL( TARGETS test-lingua RUNTIME DESTINATION share/yocto/demo)

ENABLE_TESTING()

ADD_TEST(token test-lingua token)
