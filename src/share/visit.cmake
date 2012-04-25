# Wrapper to use VISIT, parallel !

SET(VISIT_FOUND OFF)

########################################################################
## Looking for VisIt
########################################################################
SET(VISIT $ENV{VISIT} CACHE STRING "VISIT path" )

IF(   "" STREQUAL "${VISIT}" )
	MESSAGE( STATUS "No VisIt detected" )
ELSE( "" STREQUAL "${VISIT}" )
	####################################################################
	# MPI is required for parallel engines
	####################################################################
	FIND_PACKAGE(MPI REQUIRED)
	SET(VISIT_FOUND ON)
	MESSAGE( STATUS "Found VisIt in ${VISIT}" )
	
	####################################################################
	# Tuning variables for libsim
	####################################################################
	SET(LIBSIM_DIR ${VISIT}/libsim/V2)
	INCLUDE_DIRECTORIES( ${LIBSIM_DIR}/include ${MPI_INCLUDE_PATH})
	LINK_DIRECTORIES( ${LIBSIM_DIR}/lib )
	
	####################################################################
	# Macro to link what's necessary
	####################################################################
	MACRO(TARGET_LINK_VISIT tgt )
		MESSAGE( STATUS "VisIt --> ${tgt}" )
		
		###################
		## Append libsimV2
		###################
		TARGET_LINK_LIBRARIES( ${tgt} simV2 simV2f y-visit y-mpi ${MPI_LIBRARIES} )
		
		#########################
		## Append optional libdl
		#########################
		SET(VISIT_DL OFF )
		IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
			SET(VISIT_DL ON)
		ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
		
		IF(VISIT_DL)
			MESSAGE( STATUS "libdl --> ${tgt}" )
			TARGET_LINK_LIBRARIES( ${tgt} dl )
		ENDIF(VISIT_DL)
	ENDMACRO(TARGET_LINK_VISIT)
	
	####################################################################
	# Macro to move interfaces
	# TODO: check depending on platforms
	####################################################################
	SET(VISIT_UI_DIR "$ENV{HOME}/.visit/ui")
	MACRO(VISIT_CUSTOM_UI tgt file )
		MESSAGE( STATUS "Visit Custom UI for ${tgt} : ${file}" )
		ADD_CUSTOM_COMMAND( TARGET ${tgt}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory  "${VISIT_UI_DIR}"
        COMMAND ${CMAKE_COMMAND} ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${VISIT_UI_DIR}/loop.ui"
        COMMENT "Copy ${file} to ${VISIT_UI_DIR}"
        )
	ENDMACRO(VISIT_CUSTOM_UI)
	
	
ENDIF("" STREQUAL "${VISIT}")
