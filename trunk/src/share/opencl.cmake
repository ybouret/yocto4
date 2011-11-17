########################################################################
#
# add OpenCL include path and peprare the OPENCL_LINK_TO macro
#
########################################################################

SET( OPENCL_HAS_SDK OFF )
SET( OPENCL_ON_DARWIN OFF )

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(OPENCL_ON_DARWIN ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

SET( OPENCL_ON_LINUX  OFF )
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(OPENCL_ON_LINUX ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

SET( OPENCL_ON_WIN32 WIN32 )

IF(OPENCL_ON_DARWIN)
	MESSAGE( STATUS "[OpenCL] Using Apple System")
	SET( OPENCL_HAS_SDK ON )
	####################################################################
	##
	## Mac OS X >= 10.6 Settings
	##
	####################################################################

	#-------------------------------------------------------------------
	# CL/OpenCL.h is in the path
	#-------------------------------------------------------------------
	
	#-------------------------------------------------------------------
	# Use OpenCL framework
	#-------------------------------------------------------------------
	MACRO( OPENCL_LINK_TO tgt )
		MESSAGE( STATUS "${tgt} will use OpenCL (Apple)" )
		TARGET_LINK_LIBRARIES( ${tgt} "-framework OpenCL")
	ENDMACRO(OPENCL_LINK_TO)
	
ENDIF(OPENCL_ON_DARWIN)
	
IF( NOT OPENCL_HAS_SDK )
	####################################################################
	#
	# Look for ATI Stream SDK, any other platform
	#
	####################################################################
	SET( ATISTREAMSDKROOT $ENV{ATISTREAMSDKROOT} )
	SET( OPENCL_HAS_ATI ON )
	IF( "${ATISTREAMSDKROOT}" STREQUAL "" )
	SET( OPENCL_HAS_ATI OFF )
	ENDIF( "${ATISTREAMSDKROOT}" STREQUAL "" )
	
	IF( OPENCL_HAS_ATI )
		MESSAGE(STATUS "Found ATI" )
		SET( OPENCL_HAS_SDK ON )
		#---------------------------------------------------------------
		# register the include path
		#---------------------------------------------------------------
		INCLUDE_DIRECTORIES( "${ATISTREAMSDKROOT}/include" )
		IF( YOCTO64 )
			MESSAGE( STATUS "OpenCL 64 bits" )
			LINK_DIRECTORIES( "${ATISTREAMSDKROOT}/lib/x86_64" )
		ELSE( YOCTO64 )
			MESSAGE( STATUS "OpenCL 32 bits" )
			LINK_DIRECTORIES( "${ATISTREAMSDKROOT}/lib/x86" )
		ENDIF( YOCTO64 )
		
		#---------------------------------------------------------------
		# Use OpenCL 
		#---------------------------------------------------------------
		MACRO( OPENCL_LINK_TO tgt )
			MESSAGE( STATUS "${tgt} will use OpenCL (ATI Stream SDK)" )
			TARGET_LINK_LIBRARIES( ${tgt} "OpenCL")
		ENDMACRO(OPENCL_LINK_TO)
	ENDIF( OPENCL_HAS_ATI )
ENDIF( NOT OPENCL_HAS_SDK )

IF( NOT OPENCL_HAS_SDK AND WIN32 )
	#####################################################################
	##
	## Try to Detect Windows CUDA OpenCL, since ATI is not detected
	##
	#####################################################################
	SET(OPENCL_INCLUDE_PATH "$ENV{CUDA_INC_PATH}" )
	SET(OPENCL_LIBRARY_PATH "$ENV{CUDA_LIB_PATH}" )
	SET(OPENCL_HAS_NVIDIA ON)
	IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux"  OR "${OPENCL_INCLUDE_PATH}" STREQUAL "" )
	SET(OPENCL_HAS_NVIDIA OFF)
	ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux"  OR "${OPENCL_INCLUDE_PATH}" STREQUAL "" )
	
	IF( OPENCL_HAS_NVIDIA )
		SET(OPENCL_HAS_SDK ON)
		INCLUDE_DIRECTORIES( "${OPENCL_INCLUDE_PATH}" )
		LINK_DIRECTORIES(    "${OPENCL_LIBRARY_PATH}" )
		MACRO( OPENCL_LINK_TO tgt )
			MESSAGE( STATUS "${tgt} will use OpenCL (nVidia CUDA/Windows)" )
			TARGET_LINK_LIBRARIES( ${tgt} "OpenCL")
		ENDMACRO(OPENCL_LINK_TO)
	ENDIF( OPENCL_HAS_NVIDIA ) 
		
		
ENDIF( NOT OPENCL_HAS_SDK AND WIN32 )


IF( NOT OPENCL_HAS_SDK AND OPENCL_ON_LINUX )
	#####################################################################
	##
	##Looking for system OpenCL	
	##
	#####################################################################
	MESSAGE( STATUS "Looking for system OpenCL" )
	FIND_FILE(CL-H cl.h PATHS /usr/include/CL /usr/local/cuda/include/CL)
	MESSAGE( STATUS "cl.h=${CL-H}" )
	IF( NOT "${CL-H}" STREQUAL "CL-H-NOTFOUND"  )
		MESSAGE( STATUS "found system cl.h"  )
		GET_FILENAME_COMPONENT(OPENCL_INCLUDE_CL ${CL-H} PATH )
		GET_FILENAME_COMPONENT(OPENCL_INCLUDE    ${OPENCL_INCLUDE_CL} PATH )
		MESSAGE( STATUS "in ${OPENCL_INCLUDE}" )	
		SET(OPENCL_HAS_SDK ON )
		INCLUDE_DIRECTORIES( ${OPENCL_INCLUDE} )
		MACRO( OPENCL_LINK_TO tgt )
			MESSAGE( STATUS "${tgt} will use OpenCL (Linux/System)" )
			TARGET_LINK_LIBRARIES( ${tgt} "OpenCL")
		ENDMACRO(OPENCL_LINK_TO)
	ENDIF(  NOT "${CL-H}" STREQUAL "CL-H-NOTFOUND" )
		
ENDIF( NOT OPENCL_HAS_SDK AND OPENCL_ON_LINUX )


## Let the user decide...
SET(OPENCL_FOUND ON)
IF( NOT OPENCL_HAS_SDK )
 SET(OPENCL_FOUND OFF)
 MESSAGE( STATUS "No OpenCL support was found" )
ENDIF( NOT OPENCL_HAS_SDK )
