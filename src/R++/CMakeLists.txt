INCLUDE(../share/R.cmake)
INCLUDE_DIRECTORIES(. ../main ../mpa)

IF(R_FOUND)

IF(WIN32)
	SET(THE_DLL "rcode.dll")
ELSE()
	SET(THE_DLL "rcode.so")
ENDIF()

#How to use R to build a module...

#include are in Makevars
ADD_CUSTOM_TARGET(rcode ALL
COMMAND
${R_EXE} CMD SHLIB -c --preclean -o ${CMAKE_CURRENT_SOURCE_DIR}/${THE_DLL} ${CMAKE_CURRENT_SOURCE_DIR}/rcode.cpp "$<TARGET_FILE:yocto>"
	COMMENT "Building rtest .Call module: ${THE_DLL}"
	SOURCES rcode.cpp rcode.R yocto/R++/com.hpp
	DEPENDS yocto
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ENDIF(R_FOUND)

INSTALL( FILES
yocto/R++/com.hpp
DESTINATION include/yocto/R++)
