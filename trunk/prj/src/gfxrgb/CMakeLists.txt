PROJECT(fltk)

INCLUDE(${SDK}/share/yocto/fltk.cmake)

IF(FLTK_FOUND)
	MESSAGE( STATUS "[[ Adding GFX/rgb ]]" )
	FLUID_UIC(rgb)
	ADD_EXECUTABLE(gfxrgb  main.cpp ${rgb_SOURCES})
	TARGET_LINK_YOCTO(gfxrgb gfx)
	TARGET_LINK_FLTK(gfxrgb)
ENDIF()


