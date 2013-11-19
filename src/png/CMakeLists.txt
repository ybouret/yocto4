PROJECT(y-png C)

MESSAGE( STATUS "---------------- <png> ----------------" )


INCLUDE_DIRECTORIES(../z/yocto/zlib)


SET(PNGROOT yocto/png)

#PNGLIBCONF
SET(PNGLIBCONF ${CMAKE_CURRENT_SOURCE_DIR}/${PNGROOT}/pnglibconf.h)
MESSAGE( STATUS "using ${PNGLIBCONF}" )
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/share/pnglibconf.h.prebuilt
               ${PNGLIBCONF})

SET(decl  
	${PNGROOT}/png.h
	${PNGROOT}/pngconf.h
	${PNGLIBCONF})
	
INSTALL( FILES ${decl} DESTINATION include/yocto/png )

SET(impl
  ${PNGROOT}/pngdebug.h
  ${PNGROOT}/pnginfo.h
  ${PNGROOT}/pngpriv.h
  ${PNGROOT}/pngstruct.h
  ${PNGROOT}/png.c
  ${PNGROOT}/pngerror.c
  ${PNGROOT}/pngget.c
  ${PNGROOT}/pngmem.c
  ${PNGROOT}/pngpread.c
  ${PNGROOT}/pngread.c
  ${PNGROOT}/pngrio.c
  ${PNGROOT}/pngrtran.c
  ${PNGROOT}/pngrutil.c
  ${PNGROOT}/pngset.c
  ${PNGROOT}/pngtrans.c
  ${PNGROOT}/pngwio.c
  ${PNGROOT}/pngwrite.c
  ${PNGROOT}/pngwtran.c
  ${PNGROOT}/pngwutil.c
)


SET(srcs ${decl} ${impl})

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

ADD_LIBRARY(y-png STATIC ${srcs})
INSTALL( TARGETS y-png ARCHIVE DESTINATION lib )
ADD_SUBDIRECTORY(tests)

MESSAGE( STATUS "---------------- </png> ---------------" )
MESSAGE( STATUS "" )

