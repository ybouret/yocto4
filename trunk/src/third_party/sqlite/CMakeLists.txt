add_definitions(-DSQLITE_ENABLE_RTREE)
IF(YOCTO_GNU)
	add_definitions(-std=c99)
ENDIF()

ADD_LIBRARY(y-sqlite STATIC sqlite3.c sqlite3.h sqlite3ext.h)

INSTALL( FILES sqlite3.h sqlite3ext.h DESTINATION include/yocto/sqlite )
INSTALL( TARGETS y-sqlite ARCHIVE DESTINATION lib ) 
