PROJECT(Python)

FIND_PACKAGE(PythonLibs)

IF(PYTHONLIBS_FOUND)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} ../main .)

FILE( GLOB decl yocto/python/*.hpp )
FILE( GLOB impl yocto/python/*.cpp )
SET(srcs ${decl} ${impl} )

ADD_LIBRARY(y-python ${srcs})

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

ENDIF(PYTHONLIBS_FOUND)




