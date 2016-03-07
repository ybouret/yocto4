
SET(decl yocto/jbig/jbig.h yocto/jbig/jbig_ar.h)
SET(impl yocto/jbig/jbig.c yocto/jbig/jbig_ar.c)
SET(srcs ${decl} ${impl})
ADD_LIBRARY(jbig ${srcs})
