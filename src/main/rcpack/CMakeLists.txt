ADD_EXECUTABLE(yocto-rcpack rcpack.cpp)
TARGET_LINK_YOCTO(yocto-rcpack)

ADD_EXECUTABLE(rcdemo main.cpp)
TARGET_LINK_YOCTO(rcdemo)

ADD_DEPENDENCIES(rcdemo yocto-rcpack)

SET(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources)
MESSAGE( STATUS "resources@${RESOURCES}" )

ADD_CUSTOM_COMMAND(
	TARGET rcdemo 
	POST_BUILD
	COMMAND yocto-rcpack $<TARGET_FILE:rcdemo> ${RESOURCES}
	COMMENT "Patching  rcdemo"  )
	
INSTALL( TARGETS yocto-rcpack RUNTIME DESTINATION bin )
