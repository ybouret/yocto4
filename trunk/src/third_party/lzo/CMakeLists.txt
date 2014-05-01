PROJECT(lzo)

SET( decl 
	yocto/lzo/minilzo.h 
	yocto/lzo/lzoconf.h 
	yocto/lzo/lzodefs.h)
	
SET( srcs yocto/lzo/minilzo.c ${decl} )

INSTALL( FILES ${decl} DESTINATION include/yocto/lzo)

SOURCE_GROUP( lzo FILES ${srcs} )

ADD_LIBRARY(y-lzo STATIC ${srcs})

INSTALL( TARGETS y-lzo ARCHIVE DESTINATION lib )

IF(NOT MSVC)

ADD_EXECUTABLE(test-lzo yocto/lzo/testmini.c)
TARGET_LINK_LIBRARIES(test-lzo y-lzo)

ADD_CUSTOM_COMMAND( 
	TARGET  test-lzo
	POST_BUILD
	COMMAND test-lzo
	COMMENT "Auto Testing LZO..."
)

ENDIF(NOT MSVC)

