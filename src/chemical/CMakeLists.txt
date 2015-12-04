
FILE( GLOB decl yocto/chemical/*.hpp )
FILE( GLOB impl yocto/chemical/*.cpp )
SET(species_gram yocto/chemical/species.g)
SET(species_incl yocto/chemical/species.inc)


#ADD_CUSTOM_COMMAND(
#OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${species_incl}
#COMMAND file2data ${CMAKE_CURRENT_SOURCE_DIR}/${species_gram} ${CMAKE_CURRENT_SOURCE_DIR}/${species_incl}
#DEPENDS file2data ${CMAKE_CURRENT_SOURCE_DIR}/${species_gram}
#COMMENT "Generating ${species_incl}"
#)

YOCTO_FILE2DATA(${species_gram} ${species_incl})
YOCTO_FILE2DATA(yocto/chemical/level1.g yocto/chemical/level1.inc)


SET(srcs ${decl} ${impl} ${species_gram} ${species_incl}
yocto/chemical/level1.g yocto/chemical/level1.inc
)
SOURCE_GROUP(chemical FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../lang ../mk ../third_party/lua/yocto/lua ../lua++ )

ADD_LIBRARY(y-chemical STATIC ${srcs} )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

