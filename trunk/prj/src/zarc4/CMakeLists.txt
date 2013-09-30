PROJECT(zarc4)
INCLUDE(${SDK}/share/yocto/fltk.cmake)


IF(FLTK_FOUND)
	#FLUID_UIC(hello)
	ADD_EXECUTABLE(zarc4 zarc4.cpp ${zarc4_SOURCES})
	TARGET_LINK_YOCTO(zarc4 z z++ kr)
	TARGET_LINK_FLTK(zarc4)
    INSTALL( TARGETS zarc4 RUNTIME DESTINATION bin )
ENDIF()


