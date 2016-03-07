IF(MSVC)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

SET(decl yocto/jbig/jbig.h yocto/jbig/jbig_ar.h)
SET(impl yocto/jbig/jbig.c yocto/jbig/jbig_ar.c)
SET(srcs ${decl} ${impl})
ADD_LIBRARY(y-jbig ${srcs})

ADD_EXECUTABLE(test-jbig yocto/jbig/tstcodec.c)
TARGET_LINK_LIBRARIES(test-jbig y-jbig)

