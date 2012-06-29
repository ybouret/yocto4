ADD_EXECUTABLE(yocto-rcpack rcpack.cpp)
TARGET_LINK_YOCTO(yocto-rcpack fs)

ADD_EXECUTABLE(rcdemo main.cpp)
TARGET_LINK_YOCTO(rcdemo)

ADD_DEPENDENCIES(rcdemo yocto-rcpack)

GET_TARGET_PROPERTY(the_exe rcdemo LOCATION )
MESSAGE( STATUS "patching ${the_exe}" )

SET(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources)
MESSAGE( STATUS "resources@${RESOURCES}" )

ADD_CUSTOM_COMMAND(
	TARGET rcdemo 
	POST_BUILD
	COMMAND yocto-rcpack ${the_exe} ${RESOURCES} )
	
INSTALL( TARGETS yocto-rcpack RUNTIME DESTINATION bin )
