PROJECT(chem)

FILE( GLOB decl yocto/chemical/*.hpp)
FILE( GLOB impl yocto/chemical/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/chemical )
SOURCE_GROUP( chemical FILES ${srcs} )

FILE( GLOB decl_lua yocto/chemical/lua/*.hpp)
FILE( GLOB impl_lua yocto/chemical/lua/*.cpp)
SET( srcs_lua ${decl_lua} ${impl_lua} )
INSTALL( FILES ${decl_lua} DESTINATION include/yocto/chemical/lua )
SOURCE_GROUP( chemical/lua FILES ${srcs_lua} )

FILE( GLOB srcs_boot yocto/chemical/boot/*.cpp)
SOURCE_GROUP( chemical/boot FILES ${srcs_boot} )
INCLUDE_DIRECTORIES( . ../main ../mk ../third_party/lua ../lua++ )

FILE( GLOB srcs_eqs yocto/chemical/eqs/*.cpp )
SOURCE_GROUP( chemical/eqs FILES ${srcs_eqs} )

FILE( GLOB srcs_eff yocto/chemical/eff/*.cpp )
SOURCE_GROUP( chemical/eff FILES ${srcs_eff} )



ADD_LIBRARY( y-chem STATIC ${srcs} ${srcs_lua} ${srcs_boot} ${srcs_eqs} )
INSTALL( TARGETS y-chemical ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

