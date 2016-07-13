FILE(GLOB test_srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-gfx png tiff)

#ENABLE_TESTING()


