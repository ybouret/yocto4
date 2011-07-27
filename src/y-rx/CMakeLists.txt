PROJECT(y-rx)

FILE( GLOB rx_decl yocto/rx/*.hpp)
FILE( GLOB rx_impl yocto/rx/*.cpp)
SET( rx_srcs ${rx_decl} ${rx_impl} )
INSTALL( FILES ${rx_decl} DESTINATION include/yocto/rx )
SOURCE_GROUP( rx FILES ${rx_srcs} )

FILE( GLOB pat_decl yocto/rx/pattern/*.hpp)
FILE( GLOB pat_impl yocto/rx/pattern/*.cpp)
SET( pat_srcs ${pat_decl} ${pat_impl} )
INSTALL( FILES ${rx_decl} DESTINATION include/yocto/rx/pattern )
SOURCE_GROUP( rx/pattern FILES ${pat_srcs} )


INCLUDE_DIRECTORIES( . ../y-main )


ADD_LIBRARY( y-rx STATIC ${rx_srcs} ${pat_srcs} )
INSTALL( TARGETS y-rx ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

