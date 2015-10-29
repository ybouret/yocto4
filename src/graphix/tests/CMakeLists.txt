FILE(GLOB test_srcs test-*.cpp)

ADD_EXECUTABLE(test-graphix main.cpp ${test_srcs})
TARGET_LINK_YOCTO(test-graphix graphix jpeg png z)

IF(YOCTO_LINUX)
	TARGET_LINK_LIBRARIES(test-graphix dl)
ENDIF()

ENABLE_TESTING()
