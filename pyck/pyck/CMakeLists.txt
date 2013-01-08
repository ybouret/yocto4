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
	runnable.hpp	runnable.cpp
	team.hpp
	c_array.hpp
	pair.hpp
)

ADD_LIBRARY( pyck STATIC ${pyck_srcs} )

ADD_EXECUTABLE(test-pyck main.cpp)
TARGET_LINK_LIBRARIES(test-pyck pyck)

IF( CMAKE_SYSTEM_NAME MATCHES "Linux|FreeBSD" )
	TARGET_LINK_LIBRARIES(test-pyck pthread)
ENDIF()

INCLUDE(../R.cmake)
IF(R_FOUND)
	GET_TARGET_PROPERTY(pyckLib pyck LOCATION )
	MESSAGE( STATUS "pyckLib=${pyckLib}" )

	ADD_CUSTOM_TARGET(pyck_R ALL
        COMMAND ${R_EXE} CMD SHLIB -c --preclean -o ${CMAKE_CURRENT_SOURCE_DIR}/pyck_R.so ${CMAKE_CURRENT_SOURCE_DIR}/pyck_R.cpp "${pyckLib}"
        COMMENT "Building .Call wink module"
        DEPENDS pyck 
        SOURCES pyck_R.cpp)
ENDIF()
