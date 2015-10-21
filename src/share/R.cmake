######################################
##
## Finding R
##
#####################################
SET(R_FOUND OFF)

IF(MSVC)
	MESSAGE( STATUS "Disabling R with MSCV")
	SET(R_EXE "R_EXE-NOTFOUND")
ELSE(MSVC)
	MESSAGE( STATUS "Looking for R...")
	FIND_PROGRAM(R_EXE "R")
ENDIF(MSVC)

IF("${R_EXE}" STREQUAL "R_EXE-NOTFOUND")
	# no R was installed on this system
        MESSAGE( STATUS "R is not available")
ELSE("${R_EXE}" STREQUAL "R_EXE-NOTFOUND")
	# ok, we found R
        MESSAGE( STATUS "R is ${R_EXE}")
        SET(R_FOUND ON)
ENDIF("${R_EXE}" STREQUAL "R_EXE-NOTFOUND")

