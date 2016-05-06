
FILE(GLOB xz_impl *.c)
FILE(GLOB xz_decl *.h)

SET(xz_srcs ${xz_decl} ${xz_impl})

#ADD_DEFINITIONS( -DHAVE_CONFIG_H )
INCLUDE_DIRECTORIES(../../main)

ADD_LIBRARY( y-lzma STATIC ${xz_srcs} )
