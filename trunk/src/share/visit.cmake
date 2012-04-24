# Wrapper to use VISIT, parallel !

SET(VISIT_FOUND OFF)

SET(VISIT $ENV{VISIT} CACHE STRING "VISIT path" )

IF(   "" STREQUAL "${VISIT}" )
	MESSAGE( STATUS "No VisIt detected" )
ELSE( "" STREQUAL "${VISIT}" )
	FIND_PACKAGE(MPI REQUIRED)
	SET(VISIT_FOUND ON)
	MESSAGE( STATUS "Found VisIt in ${VISIT}" )
	#INCLUDE(${VISIT}/include/VisItLibraryDependencies.cmake)
	SET(LIBSIM_DIR ${VISIT}/libsim/V2)
	INCLUDE_DIRECTORIES( ${LIBSIM_DIR}/include ${MPI_INCLUDE_PATH})
	LINK_DIRECTORIES( ${LIBSIM_DIR}/lib )
	
	MACRO(TARGET_LINK_VISIT tgt )
		MESSAGE( STATUS "VisIt --> ${tgt}" )
		
		## Append libsimV2
		TARGET_LINK_LIBRARIES( ${tgt} simV2 simV2f y-mpi ${MPI_LIBRARIES} )
		
		## Append optional libdl
		SET(VISIT_DL OFF )
		IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
			SET(VISIT_DL ON)
		ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
		
		IF(VISIT_DL)
			MESSAGE( STATUS "libdl --> ${tgt}" )
			TARGET_LINK_LIBRARIES( ${tgt} dl )
		ENDIF(VISIT_DL)
	ENDMACRO(TARGET_LINK_VISIT)
	
ENDIF("" STREQUAL "${VISIT}")
