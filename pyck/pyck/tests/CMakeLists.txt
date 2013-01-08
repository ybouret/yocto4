ADD_EXECUTABLE(test-pyck main.cpp)
TARGET_LINK_LIBRARIES(test-pyck pyck)

IF( CMAKE_SYSTEM_NAME MATCHES "Linux|FreeBSD" )
	TARGET_LINK_LIBRARIES(test-pyck pthread)
ENDIF()
