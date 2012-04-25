
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-visit main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-visit )
TARGET_LINK_VISIT( test-visit )
VISIT_CUSTOM_UI( test-visit loop.ui )

