
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-net main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-net net)
IF(WIN32)
	IF( "${CC_NAME}" MATCHES "gcc.*" )
		TARGET_LINK_LIBRARIES( test-net ws2_32 )
	ENDIF()
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  TARGET_LINK_LIBRARIES(test-net socket nsl)
ENDIF()

ENABLE_TESTING()
ADD_TEST(GetHostName test-net get_host_name)

