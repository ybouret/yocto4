PROJECT(bz2 C)

SET( srcs
	yocto/bz2/bzlib.h 
	yocto/bz2/bzlib_private.h
	yocto/bz2/blocksort.c 
	yocto/bz2/huffman.c   
	yocto/bz2/crctable.c  
	yocto/bz2/randtable.c 
	yocto/bz2/compress.c  
	yocto/bz2/decompress.c
	yocto/bz2/bzlib.c
)

ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_CRT_SECURE_NO_WARNINGS)
ADD_LIBRARY(y-bz2 STATIC ${srcs})
SOURCE_GROUP( bz2 FILES ${srcs} )
INSTALL( FILES   yocto/bz2/bzlib.h DESTINATION include/yocto/bz2 )
INSTALL( TARGETS y-bz2     ARCHIVE DESTINATION lib )

#Set to TRUE to test bz2
IF(FALSE)

ADD_EXECUTABLE(y_bzip2 yocto/bz2/bzip2.c)
TARGET_LINK_LIBRARIES(y_bzip2 y-bz2)

SET(file1 ${CMAKE_CURRENT_SOURCE_DIR}/yocto/bz2/sample1.ref)
SET(file2 ${CMAKE_CURRENT_SOURCE_DIR}/yocto/bz2/sample2.ref)
SET(file3 ${CMAKE_CURRENT_SOURCE_DIR}/yocto/bz2/sample3.ref)

ADD_CUSTOM_COMMAND(
	TARGET  y_bzip2
	POST_BUILD
	COMMAND y_bzip2 ARGS -v -1 < ${file1} > sample1.bz2
	COMMAND y_bzip2 ARGS -v -d < sample1.bz2 > sample1.tst
	COMMAND ${CMAKE_COMMAND} ARGS -E compare_files sample1.tst ${file1}
	COMMAND y_bzip2 ARGS -v -2 < ${file2} > sample2.bz2
	COMMAND y_bzip2 ARGS -v -d < sample2.bz2 > sample2.tst
	COMMAND ${CMAKE_COMMAND} ARGS -E compare_files sample2.tst ${file2}
	COMMAND y_bzip2 ARGS -v -3  < ${file3} > sample3.bz2
	COMMAND y_bzip2 ARGS -v -ds < sample3.bz2 > sample3.tst
	COMMAND ${CMAKE_COMMAND} ARGS -E compare_files sample3.tst ${file3}
	COMMENT "Testing y_bzip2" )
ENDIF()
	
