########################################################################
##
## Simplified FLTK configuration, based on fltk-config
##
########################################################################

MESSAGE( STATUS "<FLTK>" )

########################################################################
##
## fltk-config and fluid
##
########################################################################
SET(FLTK_FOUND OFF)

SET(FLTK_BIN $ENV{FLTK_BIN} CACHE STRING "Override fltk-config location" )
IF( "" STREQUAL "${FLTK_BIN}" )
	
	# automatic detection
	MESSAGE( STATUS "Looking for default FLTK")
	FIND_PROGRAM( FLTK-CONFIG fltk-config )
	IF( ${FLTK-CONFIG} STREQUAL FLTK-CONFIG-NOTFOUND )
		MESSAGE( STATUS "Can't find fltk-config" )
	ELSE()
		# got fltk config
		FIND_PROGRAM( FLUID fluid )
		IF( ${FLUID} STREQUAL FLUID-NOTFOUND )
			MESSAGE( STATUS "Can't find fluid" )
		ELSE()
			# got fluid
			SET(FLTK_FOUND ON)
		ENDIF()
	ENDIF()
	
	

ELSE()
	# manual override
	SET(FLTK-CONFIG "${FLTK_BIN}/fltk-config" )
	SET(FLUID       "${FLTK_BIN}/fluid"       )
	SET(FLTK_FOUND ON)
ENDIF()

IF(FLTK_FOUND)

MESSAGE( STATUS "  @FLTK fltk-config='${FLTK-CONFIG}'" )
MESSAGE( STATUS "  @FLTK fluid='${FLUID}" )

########################################################################
##
## Compilation
##
########################################################################

#-----------------------------------------------------------------------
# get cxxflags 
#-----------------------------------------------------------------------
MESSAGE( STATUS "  @FLTK query cxxflags..." )
EXEC_PROGRAM( bash ARGS ${FLTK-CONFIG} --cxxflags OUTPUT_VARIABLE FLTK-CXXFLAGS)
MESSAGE( STATUS "  @FLTK-CXXFLAGS='${FLTK-CXXFLAGS}'" )

#-----------------------------------------------------------------------
# extract include directories
#-----------------------------------------------------------------------
STRING( REGEX MATCHALL "[-][I]([^ ;])+" FLTK-INCLUDES ${FLTK-CXXFLAGS} )
STRING( REPLACE  "-I" "" FLTK-INCLUDES "${FLTK-INCLUDES}")
MESSAGE( STATUS "  @FLTK-INCLUDES='${FLTK-INCLUDES}'" )
INCLUDE_DIRECTORIES(${FLTK-INCLUDES})

#-----------------------------------------------------------------------
# extract definitions
#-----------------------------------------------------------------------
STRING( REGEX MATCHALL "[-][D]([^ ;])+" FLTK-DEFINES ${FLTK-CXXFLAGS} )
MESSAGE( STATUS "  @FLTK-DEFINES='${FLTK-DEFINES}'")
ADD_DEFINITIONS( ${FLTK-DEFINES} )

#-----------------------------------------------------------------------
# extract FLTK link directory
#-----------------------------------------------------------------------
EXEC_PROGRAM( bash ARGS ${FLTK-CONFIG} --ldflags OUTPUT_VARIABLE FLTK-LDFLAGS)
MESSAGE( STATUS "  @FLTK-LDFLAGS='${FLTK-LDFLAGS}'")
STRING( REGEX MATCHALL "[-][L]([^ ;])+" FLTK-LINK-DIR ${FLTK-LDFLAGS} )
STRING( REPLACE  "-L" "" FLTK-LINK-DIR "${FLTK-LINK-DIR}")
MESSAGE( STATUS "  @FLTK-LINK-DIR='${FLTK-LINK-DIR}'" )
LINK_DIRECTORIES( ${FLTK-LINK-DIR} )

########################################################################
## Linking
########################################################################
## Link with optional FLTK libraries
MACRO(TARGET_LINK_FLTK THE_TARGET)

	#initialize arguments for fltk-config
	MESSAGE( STATUS "${THE_TARGET} will use FLTK libraries" )
	SET(_fltk_ldflags "--ldflags" )

	#collect extra arguments (images,gl,...)
	FOREACH( extra ${ARGN} )
		SET(_fltk_ldflags "${_fltk_ldflags} --use-${extra}" )
	ENDFOREACH( extra ${ARGN} )

	#get all args
	MESSAGE( STATUS "(with 'fltk-config ${_fltk_ldflags}')" )
	EXEC_PROGRAM( bash ARGS ${FLTK-CONFIG} ${_fltk_ldflags} OUTPUT_VARIABLE FLTK-LDFLAGS)
	#MESSAGE( STATUS "FLTK-LDFLAGS='${FLTK-LDFLAGS}'")

	#extract libraries, link directories is already set
	STRING( REGEX MATCHALL "([-][l]([^ ;])+)|(-framework ([^ ;])+)" FLTK-LIBS ${FLTK-LDFLAGS})
	STRING( REPLACE  "-l" "" FLTK-LIBS "${FLTK-LIBS}")
	MESSAGE( STATUS "${THE_TARGET} needs '${FLTK-LIBS}'")
	
	#and declare libraries to be linked !
	TARGET_LINK_LIBRARIES(${THE_TARGET} ${FLTK-LIBS})
ENDMACRO(TARGET_LINK_FLTK)

########################################################################
## fluid compiler API
## FLUID_UIC(UI [PATH]) will produce UI_SOURCE UI_HEADER UI_SOURCES
########################################################################
MACRO(FLUID_UIC THE_UI)
	SET(_UI_FL     "${CMAKE_CURRENT_SOURCE_DIR}/${THE_UI}.fl" )
	SET(_UI_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${THE_UI}.cxx")
	SET(_UI_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${THE_UI}.h")
	MESSAGE( STATUS "**** <${THE_UI}> will compile"   )
	MESSAGE( STATUS "** fluid : ${_UI_FL}"     )
	MESSAGE( STATUS "** source: ${_UI_SOURCE}" )
	MESSAGE( STATUS "** header: ${_UI_HEADER}" )
	
	#create the command
	ADD_CUSTOM_COMMAND( OUTPUT  ${_UI_SOURCE} ${_UI_HEADER}
	                    COMMAND ${FLUID} -c -o ${_UI_SOURCE} -h ${_UI_HEADER} ${_UI_FL} 
						DEPENDS ${_UI_FL} )
	
	#register the output directory	
	INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

	#build the global variables
	SET( ${THE_UI}_SOURCE  ${_UI_SOURCE} )
	SET( ${THE_UI}_HEADER  ${_UI_HEADER} )
	SET( ${THE_UI}_SOURCES ${_UI_SOURCE} ${_UI_HEADER})
ENDMACRO(FLUID_UIC)

ENDIF(FLTK_FOUND)

MESSAGE( STATUS "</FLTK>" )
