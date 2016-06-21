
FILE(GLOB decl yocto/lingua/*.hpp)
FILE(GLOB impl yocto/lingua/*.cpp)
SET(srcs ${decl} ${impl})
SOURCE_GROUP(lingua FILES ${srcs})
INSTALL(FILES ${decl} DESTINATION include/yocto/lingua)

FILE(GLOB pattern_decl yocto/lingua/pattern/*.hpp)
FILE(GLOB pattern_impl yocto/lingua/pattern/*.cpp)
SET(pattern_srcs ${pattern_decl} ${pattern_impl} )
SOURCE_GROUP(lingua/pattern FILES ${pattern_srcs} )
INSTALL(FILES ${pattern_decl} DESTINATION include/yocto/lingua/pattern )

FILE(GLOB lexical_decl yocto/lingua/lexical/*.hpp)
FILE(GLOB lexical_impl yocto/lingua/lexical/*.cpp)
SET(lexical_srcs ${lexical_decl} ${lexical_impl} )
SOURCE_GROUP(lingua/lexical FILES ${lexical_srcs} )
INSTALL(FILES ${lexical_decl} DESTINATION include/yocto/lingua/lexical )

FILE(GLOB lexical_plugin_decl yocto/lingua/lexical/plugin/*.hpp)
FILE(GLOB lexical_plugin_impl yocto/lingua/lexical/plugin/*.cpp)
SET(lexical_plugin_srcs ${lexical_plugin_decl} ${lexical_plugin_impl} )
SOURCE_GROUP(lingua/lexical/plugin FILES ${lexical_plugin_srcs} )
INSTALL(FILES ${lexical_plugin_decl} DESTINATION include/yocto/lingua/lexical/plugin )

FILE(GLOB syntax_decl yocto/lingua/syntax/*.hpp)
FILE(GLOB syntax_impl yocto/lingua/syntax/*.cpp)
SET(syntax_srcs ${syntax_decl} ${syntax_impl} )
SOURCE_GROUP(lingua/syntax FILES ${syntax_srcs} )
INSTALL(FILES ${syntax_decl} DESTINATION include/yocto/lingua/syntax )

FILE(GLOB prv_decl yocto/lingua/prv/*.hpp)
FILE(GLOB prv_impl yocto/lingua/prv/*.cpp)
SET(prv_srcs ${prv_decl} ${prv_impl} )
SOURCE_GROUP(lingua/prv FILES ${prv_srcs} )

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY(y-lingua STATIC ${srcs} ${pattern_srcs} ${lexical_srcs} ${lexical_plugin_srcs} ${syntax_srcs} ${prv_srcs})

INSTALL( TARGETS y-lingua ARCHIVE DESTINATION lib )


IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()
ADD_SUBDIRECTORY(gram2def)

