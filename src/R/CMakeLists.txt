INCLUDE(../share/R.cmake)

IF(R_FOUND)

IF(WIN32)
	SET(THE_DLL "rtest.dll")
ELSE()
	SET(THE_DLL "rtest.so")
ENDIF()

#How to use R to build a module...
#INCLUDE_DIRECTORIES(. ../main)

GET_TARGET_PROPERTY(yoctoLib yocto LOCATION )

ADD_CUSTOM_TARGET(rtest ALL
COMMAND
${R_EXE} CMD SHLIB -c --preclean -o ${CMAKE_CURRENT_SOURCE_DIR}/${THE_DLL} ${CMAKE_CURRENT_SOURCE_DIR}/rtest.cpp ${yoctoLib}
	COMMENT "Building rtest .Call module: ${THE_DLL}"
	SOURCES rtest.cpp rtest.R yocto/R/R++.hpp yocto/R/IR.hpp
	DEPENDS yocto
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
#-I${CMAKE_CURRENT_SOURCE_DIR}/../main -I${CMAKE_CURRENT_SOURCE_DIR}

ENDIF(R_FOUND)

INSTALL( FILES
yocto/R/R++.hpp
yocto/R/IR.hpp
DESTINATION include/yocto/R)
