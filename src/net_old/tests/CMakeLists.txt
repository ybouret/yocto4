
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-net-old main.cpp ${test_srcs})
TARGET_LINK_YOCTO( test-net-old net-old)
IF(WIN32)
	IF( "${CC_NAME}" MATCHES "gcc.*" )
		TARGET_LINK_LIBRARIES( test-net ws2_32 )
	ENDIF()
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  TARGET_LINK_LIBRARIES(test-net socket nsl)
ENDIF()

ENABLE_TESTING()
ADD_TEST(Resolve test-net resolve)

#ADD_CUSTOM_TARGET(check-net COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS test-net)
