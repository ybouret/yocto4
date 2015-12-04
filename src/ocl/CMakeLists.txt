INCLUDE(../share/opencl.cmake)

IF(OPENCL_FOUND)

PROJECT(ocl)

FILE( GLOB ocl_decl yocto/ocl/*.hpp)
FILE( GLOB ocl_impl yocto/ocl/*.cpp)
SET( ocl_srcs ${ocl_decl} ${ocl_impl} )
INSTALL( FILES ${ocl_decl} DESTINATION include/yocto/ocl )
SOURCE_GROUP( ocl FILES ${ocl_srcs} )



SET( decl ${ocl_decl}  )
SET( srcs ${ocl_srcs}  )

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY(y-ocl STATIC ${srcs} )

INSTALL( TARGETS y-ocl ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

ENDIF(OPENCL_FOUND)
