ADD_EXECUTABLE(y_rcpack rcpack.cpp)
TARGET_LINK_YOCTO(y_rcpack)

GET_TARGET_PROPERTY(the_exe y_rcpack LOCATION )
MESSAGE( STATUS "producing ${the_exe}" )

SET(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources)
MESSAGE( STATUS "resources@${RESOURCES}" )

ADD_CUSTOM_COMMAND(
	TARGET y_rcpack
	POST_BUILD
	COMMAND y_rcpack ${the_exe} )
	
