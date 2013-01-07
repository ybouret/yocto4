SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

SET(pyck_srcs
	os.hpp		os.cpp
	error.hpp	error.cpp
	exception.hpp 	exception.cpp
	lockable.hpp	lockable.cpp
	mutex.hpp	mutex.cpp
	wtime.hpp	wtime.cpp
	sort.hpp	sort.cpp
	rand32.hpp	rand32.cpp
	thread.hpp	thread.cpp
	runnable.hpp	runnable.cpp	team.hpp
	c_array.hpp
)

ADD_LIBRARY( pyck STATIC ${pyck_srcs} )

ADD_EXECUTABLE(test-pyck main.cpp)
TARGET_LINK_LIBRARIES(test-pyck pyck)

IF( CMAKE_SYSTEM_NAME MATCHES "Linux" )
	TARGET_LINK_LIBRARIES(test-pyck pthread)
ENDIF()
