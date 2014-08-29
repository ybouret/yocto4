FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang main.cpp ${srcs})
TARGET_LINK_YOCTO( test-lang lang )
