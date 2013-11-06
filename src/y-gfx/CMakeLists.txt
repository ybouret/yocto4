PROJECT(y-gfx)

FILE( GLOB decl yocto/gfx/*.hpp)
FILE( GLOB impl yocto/gfx/*.cpp)

FILE( GLOB img_decl yocto/gfx/image/*.hpp )
FILE( GLOB img_impl yocto/gfx/image/*.cpp )

SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

SET( img_srcs ${img_decl} ${img_impl} )
INSTALL( FILES ${img_decl} DESTINATION include/yocto/gfx/image )
SOURCE_GROUP( gfx/image FILES ${img_srcs} )

INCLUDE_DIRECTORIES( . ../y-main ../y-jpeg)

ADD_EXECUTABLE(named-rgb EXCLUDE_FROM_ALL named-rgb.cpp)
TARGET_LINK_YOCTO(named-rgb)


ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
        COMMAND named-rgb ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.txt ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.txt ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.cpp
)


ADD_LIBRARY( y-gfx STATIC 	${srcs}
				${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
				${img_srcs}
)
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

