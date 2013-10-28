PROJECT(y-gfx)

FILE( GLOB decl yocto/gfx/*.hpp)
FILE( GLOB impl yocto/gfx/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/gfx )
SOURCE_GROUP( gfx FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../y-main)

ADD_EXECUTABLE(named-rgb named-rgb.cpp)
TARGET_LINK_YOCTO(named-rgb)


ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
        COMMAND named-rgb ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.txt ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/named-rgb.txt named-rgb.cpp
)


ADD_LIBRARY( y-gfx STATIC ${srcs} ${CMAKE_CURRENT_SOURCE_DIR}/yocto/gfx/named-rgb.inc)
INSTALL( TARGETS y-gfx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

