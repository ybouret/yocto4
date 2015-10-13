INCLUDE(../share/R.cmake)
INCLUDE_DIRECTORIES(. ../main ../mpa)

IF(R_FOUND)

IF(WIN32)
	SET(THE_DLL "rtest.dll")
ELSE()
	SET(THE_DLL "rtest.so")
ENDIF()

#How to use R to build a module...

#GET_TARGET_PROPERTY(yoctoLib yocto LOCATION )

#include are in Makevars
ADD_CUSTOM_TARGET(rtest ALL
COMMAND
${R_EXE} CMD SHLIB -c --preclean -o ${CMAKE_CURRENT_SOURCE_DIR}/${THE_DLL} ${CMAKE_CURRENT_SOURCE_DIR}/rtest.cpp "$<TARGET_FILE:yocto>"
	COMMENT "Building rtest .Call module: ${THE_DLL}"
	SOURCES rtest.cpp rtest.R yocto/R/R++.hpp yocto/R/IR.hpp
	DEPENDS yocto
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ENDIF(R_FOUND)

INSTALL( FILES
yocto/R/R++.hpp
yocto/R/IR.hpp
DESTINATION include/yocto/R)

ADD_EXECUTABLE(test-ir test-ir.cpp yocto/R/IR.hpp)
TARGET_LINK_YOCTO(test-ir mpa)
