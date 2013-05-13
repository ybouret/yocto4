PROJECT(y-lang)

FILE( GLOB decl yocto/lingua/*.hpp)
FILE( GLOB impl yocto/lingua/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/lingua )
SOURCE_GROUP( lingua FILES ${srcs} )

FILE( GLOB pat_decl yocto/lingua/pattern/*.hpp )
FILE( GLOB pat_impl yocto/lingua/pattern/*.cpp )
SET( pat_srcs ${pat_decl} ${pat_impl} )
INSTALL( FILES ${pat_decl} DESTINATION include/yocto/lingua/pattern )
SOURCE_GROUP( lingua/pattern FILES ${pat_srcs} )

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-lingua STATIC ${srcs} ${pat_srcs} )
INSTALL( TARGETS y-lingua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

