########################################################################
##
## SWIG automatisation
##
########################################################################
MESSAGE( STATUS "---------------- <SWIG>  ----------------" )


########################################################################
# find the swig program
########################################################################
SET(SWIG_FOUND OFF)
FIND_PROGRAM(SWIG swig)

IF( ${SWIG} STREQUAL "SWIG-NOTFOUND" )
	MESSAGE( STATUS "no SWIG available")
ELSE()
	####################################################################
	## we have swig
	####################################################################
	SET(SWIG_FOUND ON)
	MESSAGE( STATUS "[SWIG]: is ${SWIG}" )
	
	####################################################################
	# find wich kind of wrapper can be done
	####################################################################
	SET(SWIG_PYTHON OFF)
	FIND_PACKAGE(PythonLibs)
	IF(PYTHONLIBS_FOUND)
		SET(SWIG_PYTHON ON)
		MESSAGE( "[SWIG] has python" )
	ENDIF()
	
	SET(SWIG_TCL OFF)
	FIND_PACKAGE(TCL)
	IF(TCL_FOUND)
		SET(SWIG_TCL ON)
		MESSAGE( "[SWIG] has TCL" )
	ENDIF()
	
	####################################################################
	# create the swig macro: kind=python|..., source=
	####################################################################
	MACRO(SWIG_WRAPPER name kind)
		MESSAGE( STATUS "[SWIG]: generating wrapper for [${name}] in [${kind}]")
		
		################################################################
		## common variables
		################################################################
		SET(swig_output_dir   "${CMAKE_CURRENT_BINARY_DIR}/${kind}")
		SET(swig_wrapper_name "${name}_wrapper_${kind}.cpp" )
		SET(swig_wrapper_code "${swig_output_dir}/${swig_wrapper_name}")
		SET(swig_interface    "${CMAKE_CURRENT_SOURCE_DIR}/${name}.i" )
		SET(swig_user_code    "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp")
		SET(swig_target       "${name}_${kind}")
		
		################################################################
		## dependencies
		################################################################
		EXECUTE_PROCESS(
			COMMAND ${SWIG} -c++ -${kind} -MM "${swig_interface}"
			OUTPUT_VARIABLE SWIG_DEPS
			OUTPUT_STRIP_TRAILING_WHITESPACE
			)
		STRING( REGEX REPLACE "\r|\n|\r\n" ";" SWIG_DEPS "${SWIG_DEPS}" )
		STRING( REPLACE "\\;" ";" SWIG_DEPS "${SWIG_DEPS}" )
		LIST( REMOVE_AT SWIG_DEPS 0 )
		SET(SWIG_DEPENDS)
		FOREACH( D1 IN LISTS SWIG_DEPS )
			STRING( STRIP "${D1}" D2 )
			MESSAGE( STATUS "[SWIG]: depends on '${D2}'" )
			LIST( APPEND SWIG_DEPENDS "${D2}"  )
		ENDFOREACH()
		################################################################
		## generation
		################################################################
		ADD_CUSTOM_COMMAND(
			OUTPUT  "${swig_output_dir}/${swig_wrapper_name}"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${swig_output_dir}"
			COMMAND ${SWIG} -c++ -${kind} -outdir "${swig_output_dir}" -o "${swig_wrapper_code}" "${swig_interface}"
			DEPENDS ${SWIG_DEPENDS}
			)
		
		################################################################
		## default module to build
		################################################################
		ADD_LIBRARY(${swig_target} MODULE "${swig_user_code}" "${swig_wrapper_code}")
		

		################################################################
		## and its exact location
		################################################################
		GET_TARGET_PROPERTY(swig_target_name ${swig_target} LOCATION)
		
		################################################################
		##
		## python tuning
		##
		################################################################
		IF( "${kind}" STREQUAL "python" )
			IF( NOT SWIG_PYTHON )
				MESSAGE( FATAL_ERROR "[SWIG]: No python was detected!" )
			ELSE()
				MESSAGE( STATUS      "[SWIG]: tuning for [python]")
			ENDIF()
			INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
			TARGET_LINK_LIBRARIES(${swig_target} ${PYTHON_LIBRARIES})
			SET(swig_ext "so" )
			IF( WIN32 )
				SET(swig_ext "pyd")
			ENDIF()
			ADD_CUSTOM_COMMAND(
				TARGET ${swig_target}
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy "${swig_target_name}" "${swig_output_dir}/_${name}.${swig_ext}"
			)
		ENDIF()
		
		################################################################
		##
		## tcl tuning
		##
		################################################################
		IF( "${kind}" STREQUAL "tcl" )
			IF( NOT SWIG_TCL )
				MESSAGE( FATAL_ERROR "[SWIG]: No tcl was detected!" )
			ELSE()
				MESSAGE( STATUS      "[SWIG]: tuning for [tcl]")
			ENDIF()
			INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
			TARGET_LINK_LIBRARIES(${swig_target} ${TCL_LIBRARY})
			SET(swig_ext "so" )
			IF( WIN32 )
				SET(swig_ext "dll")
			ENDIF()
			ADD_CUSTOM_COMMAND(
				TARGET ${swig_target}
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy "${swig_target_name}" "${swig_output_dir}/${name}.${swig_ext}"
			)
		ENDIF()
		
	
		################################################################
		##
		## system tuning
		##
		################################################################
		SET(swig_cxx OFF)
		IF(YOCTO_SUNOS OR YOCTO_CLANG)
			SET(swig_cxx ON)
		ENDIF()	
		
		
		IF(swig_cxx)
			MESSAGE( STATUS "[SWIG]: explicit stdc++ linking" )
			TARGET_LINK_LIBRARIES(${swig_target} stdc++)
		ENDIF()
	ENDMACRO(SWIG_WRAPPER)
	
ENDIF()

MESSAGE( STATUS "---------------- </SWIG> ----------------" )
