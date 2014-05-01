PROJECT(z)

SET( decl yocto/zlib/zconf.h yocto/zlib/zlib.h )
SET( impl
  yocto/zlib/adler32.c
  yocto/zlib/compress.c
  yocto/zlib/crc32.c
  yocto/zlib/deflate.c
  yocto/zlib/gzclose.c
  yocto/zlib/gzlib.c
  yocto/zlib/gzread.c
  yocto/zlib/gzwrite.c
  yocto/zlib/inflate.c
  yocto/zlib/infback.c
  yocto/zlib/inftrees.c
  yocto/zlib/inffast.c
  yocto/zlib/trees.c
  yocto/zlib/uncompr.c
  yocto/zlib/zutil.c
  )

SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yocto/zlib )
SOURCE_GROUP( zlib FILES ${srcs} )

ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)


INCLUDE(${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)

CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
IF(HAVE_UNISTD_H)
  ADD_DEFINITIONS(-DHAVE_UNISTD_H)
ENDIF(HAVE_UNISTD_H)

ADD_LIBRARY(y-z STATIC ${srcs})
INSTALL( TARGETS y-z ARCHIVE DESTINATION lib )

#Set to TRUE to enable zlib testing
IF(NOT MSVC)
ADD_EXECUTABLE(test-z yocto/zlib/example.c)
TARGET_LINK_LIBRARIES(test-z y-z)
ADD_CUSTOM_COMMAND( TARGET test-z
	COMMAND test-z
	COMMENT "Auto Testing zlib...")
ENDIF(NOT MSVC)

