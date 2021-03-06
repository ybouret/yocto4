PROJECT(chem)

#core
FILE( GLOB decl yocto/chemical/*.hpp)
FILE( GLOB impl yocto/chemical/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/chemical )
SOURCE_GROUP( chemical FILES ${srcs} )

#equilibria specs
FILE( GLOB decl_eqs yocto/chemical/eqs/*.hpp)
FILE( GLOB impl_eqs yocto/chemical/eqs/*.cpp)
SET( srcs_eqs ${decl_eqs} ${impl_eqs} )
INSTALL( FILES ${decl_eqs} DESTINATION include/yocto/chemical/eqs )
SOURCE_GROUP( chemical/eqs FILES ${srcs_eqs} )

#encoding with lua
FILE( GLOB decl_lua yocto/chemical/lua/*.hpp)
FILE( GLOB impl_lua yocto/chemical/lua/*.cpp)
SET( srcs_lua ${decl_lua} ${impl_lua} )
INSTALL( FILES ${decl_lua} DESTINATION include/yocto/chemical/lua )
SOURCE_GROUP( chemical/lua FILES ${srcs_lua} )

INCLUDE_DIRECTORIES( . ../main ../mk ../third_party/lua/yocto/lua ../lua++ )

#specific
ADD_LIBRARY( y-chem STATIC ${srcs} ${srcs_lua} ${srcs_boot} ${srcs_eqs} )
INSTALL( TARGETS y-chem ARCHIVE DESTINATION lib )

IF(YOCTO_BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF()

