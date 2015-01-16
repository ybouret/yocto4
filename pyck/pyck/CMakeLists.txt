SET(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
SET(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

SET(pyck_srcs
	os.hpp		os.cpp
	error.hpp	error.cpp
	exception.hpp 	exception.cpp
	lockable.hpp	lockable.cpp
	mutex.hpp	mutex.cpp
	rt-clock.hpp    rt-clock.cpp
	wtime.hpp	wtime.cpp
	sort.hpp
	rand32.hpp	rand32.cpp
	thread.hpp	thread.cpp
	runnable.hpp	runnable.cpp
	file.hpp	file.cpp
	erfc.hpp	erfc.cpp
	team.hpp
	c_array.hpp
	c_matrix.hpp
	pair.hpp
	list.hpp
	pool.hpp
	stats.hpp
	R++.hpp
)

ADD_LIBRARY( pyck STATIC ${pyck_srcs} )
ADD_SUBDIRECTORY(tests)

FIND_FILE(R_CMAKE R.cmake PATHS .. NO_DEFAULT_PATH)
MESSAGE(STATUS "R_CMAKE='${R_CMAKE}'")

IF( "${R_CMAKE}" STREQUAL "R_CMAKE-NOTFOUND" )

ELSE()

	INCLUDE(${R_CMAKE})
	IF(R_FOUND)
		GET_TARGET_PROPERTY(pyckLib pyck LOCATION )
		MESSAGE( STATUS "pyckLib=${pyckLib}" )

		ADD_CUSTOM_TARGET(pyck_R ALL
        	COMMAND ${R_EXE} CMD SHLIB -c --preclean -o ${CMAKE_CURRENT_SOURCE_DIR}/tests/pyck_R.so ${CMAKE_CURRENT_SOURCE_DIR}/tests/pyck_R.cpp "${pyckLib}"
        	COMMENT "Building .Call module"
        	DEPENDS pyck 
        	SOURCES R++.hpp tests/pyck_R.cpp)
	ENDIF()

ENDIF()

