PROJECT(tiff++)

FIND_PACKAGE(TIFF)

IF(TIFF_FOUND)
	MESSAGE( STATUS "Found Tiff...")
	INCLUDE_DIRECTORIES( . ../main  ${TIFF_INCLUDE_DIR})

	ADD_LIBRARY(y-tiff MODULE 
	yocto/tiff++/interface.hpp
	yocto/tiff++/plugin.hpp 
	yocto/tiff++/interface.cpp)
	TARGET_LINK_LIBRARIES(y-tiff ${TIFF_LIBRARIES})
	INSTALL( TARGETS y-tiff LIBRARY DESTINATION lib )

ADD_SUBDIRECTORY(tests)

ENDIF(TIFF_FOUND)

