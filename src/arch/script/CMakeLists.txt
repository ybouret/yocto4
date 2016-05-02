#generated code
SET(script_gram yocto/script/script.g)
SET(script_incl yocto/script/script.inc)


ADD_CUSTOM_COMMAND(
OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${script_incl}
COMMAND file2data ${CMAKE_CURRENT_SOURCE_DIR}/${script_gram} ${CMAKE_CURRENT_SOURCE_DIR}/${script_incl}
DEPENDS file2data ${CMAKE_CURRENT_SOURCE_DIR}/${script_gram}
)

#common code
FILE( GLOB script_decl yocto/script/*.hpp)
FILE( GLOB script_impl yocto/script/*.cpp)
SET( script_srcs ${script_decl} ${script_impl} )

INSTALL( FILES ${script_decl} DESTINATION include/yocto/script )
SOURCE_GROUP( script FILES ${script_srcs} )


#gather sources
SET( srcs ${script_srcs})

INCLUDE_DIRECTORIES( . ../main ../lang)

ADD_LIBRARY( y-script STATIC ${srcs} ${script_gram} ${script_incl})
INSTALL( TARGETS y-script ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
