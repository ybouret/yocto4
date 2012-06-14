ADD_EXECUTABLE(yocto-rcpack rcpack.cpp)
TARGET_LINK_YOCTO(yocto-rcpack fs)

GET_TARGET_PROPERTY(the_exe yocto-rcpack LOCATION )
MESSAGE( STATUS "producing ${the_exe}" )

SET(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources)
MESSAGE( STATUS "resources@${RESOURCES}" )

ADD_CUSTOM_COMMAND(
	TARGET yocto-rcpack
	POST_BUILD
	COMMAND yocto-rcpack ${the_exe} ${RESOURCES} )
	
