
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-tiff++ main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-tiff++)
TARGET_LINK_LIBRARIES(test-tiff++ ${TIFF_LIBRARIES})
ADD_DEPENDENCIES(test-tiff++ y-tiff)

IF(YOCTO_LINUX)
	TARGET_LINK_LIBRARIES(test-tiff++ dl)
ENDIF()
