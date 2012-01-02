########################################################################
##
## SWIG automatisation
##
########################################################################

## find the swig program
SET(SWIG_FOUND OFF)
FIND_PROGRAM(SWIG swig)

IF( ${SWIG} STREQUAL "SWIG-NOTFOUND" )
	MESSAGE( STATUS "no SWIG available")
ELSE()
	######################################################################
	## we have swig
	######################################################################
	SET(SWIG_FOUND ON)
	MESSAGE( STATUS "[SWIG]: is ${SWIG}" )
	
	######################################################################
	# find wich kind of wrapper can be done
	######################################################################
	SET(SWIG_PYTHON OFF)
	FIND_PACKAGE(PythonLibs)
	IF(PYTHONLIBS_FOUND)
		SET(SWIG_PYTHON ON)
	ENDIF()
	
	######################################################################
	# create the swig macro: kind=python|..., source=
	######################################################################
	MACRO(SWIG_WRAPPER name kind)
		MESSAGE( STATUS "[SWIG]: generating wrapper for [${name}] in [${kind}]")
		
		## common variables
		SET(swig_output_dir   "${CMAKE_CURRENT_BINARY_DIR}/${kind}")
		SET(swig_wrapper_name "${name}_wrapper_${kind}.cpp" )
		SET(swig_wrapper_code "${swig_output_dir}/${swig_wrapper_name}")
		SET(swig_interface    "${CMAKE_CURRENT_SOURCE_DIR}/${name}.i" )
		SET(swig_user_code    "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp")
		SET(swig_target       "${name}_${kind}")
		ADD_CUSTOM_COMMAND(
			OUTPUT  "${swig_output_dir}/${swig_wrapper_name}"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${swig_output_dir}"
			COMMAND ${SWIG} -c++ -${kind} -outdir "${swig_output_dir}" -o "${swig_wrapper_code}" "${swig_interface}"
			DEPENDS "${swig_interface}"
			)
			
		## default module to build
		ADD_LIBRARY(${swig_target} MODULE "${swig_user_code}" "${swig_wrapper_code}")
		
		## and its exact location
		GET_TARGET_PROPERTY(swig_target_name ${swig_target} LOCATION)
		#MESSAGE( STATUS "swig_target_name=${swig_target_name}")
		
		## python tuning
		IF( "${kind}" STREQUAL "python" )
			IF( NOT SWIG_PYTHON )
				MESSAGE( FATAL_ERROR "[SWIG]: No python was detected!" )
			ELSE()
				MESSAGE( STATUS      "[SWIG]: tuning for [python]")
			ENDIF()
			INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
			TARGET_LINK_LIBRARIES(${swig_target} ${PYTHON_LIBRARIES})
			
			ADD_CUSTOM_COMMAND(
				TARGET ${swig_target}
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy "${swig_target_name}" "${swig_output_dir}/_${name}.so"
			)
		ENDIF()
		
	ENDMACRO(SWIG_WRAPPER)
	
ENDIF()
