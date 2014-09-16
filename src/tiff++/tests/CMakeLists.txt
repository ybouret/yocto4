
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-tiff++ main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-tiff++ tiff++)
TARGET_LINK_LIBRARIES(test-tiff++ ${TIFF_LIBRARIES})
