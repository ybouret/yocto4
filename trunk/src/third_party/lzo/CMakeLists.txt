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

#set to TRUE to enable auto-test
IF(NOT MSVC)

ADD_EXECUTABLE(testmini yocto/lzo/testmini.c)
TARGET_LINK_LIBRARIES(testmini y-lzo)

ADD_CUSTOM_COMMAND( 
	TARGET  testmini
	POST_BUILD
	COMMAND testmini
	COMMENT "Testing minilzo..."
)

ENDIF(NOT MSVC)

