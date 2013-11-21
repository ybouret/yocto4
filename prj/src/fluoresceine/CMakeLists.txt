PROJECT(fluoresceine)

INCLUDE(${SDK}/share/yocto/fltk.cmake)

IF(FLTK_FOUND)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
	MESSAGE( STATUS "[[ Adding Fluoresceine ]]" )
	FLUID_UIC(ui)
	ADD_EXECUTABLE(fluoresceine fluoresceine.cpp fluoresceine.hpp ${ui_SOURCES})
	TARGET_LINK_YOCTO(fluoresceine chemical mk)
	TARGET_LINK_FLTK(fluoresceine)
ENDIF()


