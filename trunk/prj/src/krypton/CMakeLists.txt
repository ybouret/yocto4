PROJECT(krypton)
INCLUDE(${SDK}/share/yocto/fltk.cmake)


IF(FLTK_FOUND)
	INCLUDE_DIRECTORIES(.)
	ADD_EXECUTABLE(krypton krypton.hpp krypton.cpp)
	TARGET_LINK_YOCTO(krypton z++ z kr)
	TARGET_LINK_FLTK(krypton)
	INSTALL( TARGETS krypton RUNTIME DESTINATION bin )
ENDIF()


