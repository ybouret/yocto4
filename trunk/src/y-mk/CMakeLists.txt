PROJECT(y-mk)

FILE( GLOB kernel_decl yocto/math/kernel/*.hpp)
FILE( GLOB kernel_impl yocto/math/kernel/*.cpp)
SET( kernel_srcs ${kernel_decl} ${kernel_impl} )
INSTALL( FILES ${kernel_decl} DESTINATION include/yocto/math/kernel )
SOURCE_GROUP( math/kernel FILES ${kernel_srcs} )

FILE( GLOB dat_decl yocto/math/dat/*.hpp)
FILE( GLOB dat_impl yocto/math/dat/*.cpp)
SET( dat_srcs ${dat_decl} ${dat_impl} )
INSTALL( FILES ${dat_decl} DESTINATION include/yocto/math/dat )
SOURCE_GROUP( math/dat FILES ${dat_srcs} )

FILE( GLOB fcn_decl yocto/math/fcn/*.hpp)
FILE( GLOB fcn_impl yocto/math/fcn/*.cpp)
SET( fcn_srcs ${fcn_decl} ${fcn_impl} )
INSTALL( FILES ${fcn_decl} DESTINATION include/yocto/math/fcn )
SOURCE_GROUP( math/fcn FILES ${fcn_srcs} )

FILE( GLOB opt_decl yocto/math/opt/*.hpp)
FILE( GLOB opt_impl yocto/math/opt/*.cpp)
SET( opt_srcs ${opt_decl} ${opt_impl} )
INSTALL( FILES ${opt_decl} DESTINATION include/yocto/math/opt )
SOURCE_GROUP( math/opt FILES ${opt_srcs} )

FILE( GLOB sig_decl yocto/math/sig/*.hpp)
FILE( GLOB sig_impl yocto/math/sig/*.cpp)
SET( sig_srcs ${sig_decl} ${sig_impl} )
INSTALL( FILES ${sig_decl} DESTINATION include/yocto/math/sig )
SOURCE_GROUP( math/sig FILES ${sig_srcs} )

FILE( GLOB io_decl yocto/math/io/*.hpp)
FILE( GLOB io_impl yocto/math/io/*.cpp)
SET( io_srcs ${io_decl} ${io_impl} )
INSTALL( FILES ${io_decl} DESTINATION include/yocto/math/io )
SOURCE_GROUP( math/io FILES ${io_srcs} )

SET( srcs ${kernel_srcs} ${dat_srcs} ${fcn_srcs} ${opt_srcs} ${sig_srcs} ${io_srcs})

INCLUDE_DIRECTORIES( . ../y-main )

ADD_LIBRARY( y-mk STATIC ${srcs} )
INSTALL( TARGETS y-mk ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

