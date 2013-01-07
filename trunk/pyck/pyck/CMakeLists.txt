SET(pyck_srcs
	os.hpp		os.cpp
	error.hpp	error.cpp
	exception.hpp 	exception.cpp
	lockable.hpp	lockable.cpp
	mutex.hpp	mutex.cpp
	wtime.hpp	wtime.cpp
)

ADD_LIBRARY( pyck STATIC ${pyck_srcs} )

