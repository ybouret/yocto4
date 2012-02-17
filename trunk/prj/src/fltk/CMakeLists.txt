PROJECT(fltk)

INCLUDE(${SDK}/share/yocto/fltk.cmake)

IF(FLTK_FOUND)

	MESSAGE( STATUS "[[ Adding FLTK Hello ]]" )
	FLUID_UIC(hello)
	ADD_EXECUTABLE(hello main.cpp ${hello_SOURCES})
	TARGET_LINK_YOCTO(hello)
	TARGET_LINK_FLTK(hello)
ENDIF()


