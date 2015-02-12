PROJECT(mk)

FILE( GLOB kernel_decl yocto/math/kernel/*.hpp)
FILE( GLOB kernel_impl yocto/math/kernel/*.cpp)
SET( kernel_srcs ${kernel_decl} ${kernel_impl} )
INSTALL( FILES ${kernel_decl} DESTINATION include/yocto/math/kernel )
SOURCE_GROUP( math/kernel FILES ${kernel_srcs} )

FILE( GLOB sparse_decl yocto/math/sparse/*.hpp)
FILE( GLOB sparse_impl yocto/math/sparse/*.cpp)
SET( sparse_srcs ${sparse_decl} ${sparse_impl} )
INSTALL( FILES ${sparse_decl} DESTINATION include/yocto/math/sparse )
SOURCE_GROUP( math/sparse FILES ${sparse_srcs} )

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

FILE( GLOB ode_decl yocto/math/ode/*.hpp)
FILE( GLOB ode_impl yocto/math/ode/*.cpp)
SET( ode_srcs ${ode_decl} ${ode_impl} )
INSTALL( FILES ${ode_decl} DESTINATION include/yocto/math/ode )
SOURCE_GROUP( math/ode FILES ${ode_srcs} )

FILE( GLOB ode_explicit_decl yocto/math/ode/explicit/*.hpp)
FILE( GLOB ode_explicit_impl yocto/math/ode/explicit/*.cpp)
SET( ode_explicit_srcs ${ode_explicit_decl} ${ode_explicit_impl} )
INSTALL( FILES ${ode_explicit_decl} DESTINATION include/yocto/math/ode/explicit )
SOURCE_GROUP( math/ode/explicit FILES ${ode_explicit_srcs} )

FILE( GLOB ode_implicit_decl yocto/math/ode/implicit/*.hpp)
FILE( GLOB ode_implicit_impl yocto/math/ode/implicit/*.cpp)
SET( ode_implicit_srcs ${ode_implicit_decl} ${ode_implicit_impl} )
INSTALL( FILES ${ode_implicit_decl} DESTINATION include/yocto/math/ode/implicit )
SOURCE_GROUP( math/ode/implicit FILES ${ode_implicit_srcs} )

FILE( GLOB fit_decl yocto/math/fit/*.hpp)
FILE( GLOB fit_impl yocto/math/fit/*.cpp)
SET( fit_srcs ${fit_decl} ${fit_impl} )
INSTALL( FILES ${fit_decl} DESTINATION include/yocto/math/fit )
SOURCE_GROUP( math/fit FILES ${fit_srcs} )

FILE( GLOB stat_decl yocto/math/stat/*.hpp)
FILE( GLOB stat_impl yocto/math/stat/*.cpp)
SET( stat_srcs ${stat_decl} ${stat_impl} )
INSTALL( FILES ${stat_decl} DESTINATION include/yocto/math/stat )
SOURCE_GROUP( math/stat FILES ${stat_srcs} )

FILE( GLOB alg_decl yocto/math/alg/*.hpp)
FILE( GLOB alg_impl yocto/math/alg/*.cpp)
SET( alg_srcs ${alg_decl} ${alg_impl} )
INSTALL( FILES ${alg_decl} DESTINATION include/yocto/math/alg )
SOURCE_GROUP( math/alg FILES ${alg_srcs} )

FILE( GLOB fft_decl yocto/math/fft/*.hpp)
FILE( GLOB fft_impl yocto/math/fft/*.cpp)
SET( fft_srcs ${fft_decl} ${fft_impl} )
INSTALL( FILES ${fft_decl} DESTINATION include/yocto/math/fft )
SOURCE_GROUP( math/fft FILES ${fft_srcs} )

FILE( GLOB wavelet_decl yocto/math/wavelet/*.hpp)
FILE( GLOB wavelet_impl yocto/math/wavelet/*.cpp)
SET( wavelet_srcs ${wavelet_decl} ${wavelet_impl} )
INSTALL( FILES ${wavelet_decl} DESTINATION include/yocto/math/wavelet )
SOURCE_GROUP( math/wavelet FILES ${wavelet_srcs} )

SET( srcs
${kernel_srcs}
${sparse_srcs}
${dat_srcs}
${fcn_srcs}
${opt_srcs}
${sig_srcs}
${io_srcs}
${ode_srcs}
${ode_explicit_srcs}
${ode_implicit_srcs}
${fit_srcs}
${stat_srcs}
${alg_srcs}
${fft_srcs}
${wavelet_srcs})

INCLUDE_DIRECTORIES( . ../main )

ADD_LIBRARY( y-mk STATIC ${srcs} )
INSTALL( TARGETS y-mk ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

