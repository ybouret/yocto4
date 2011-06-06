PROJECT(y-main)
FILE( GLOB top_decl yocto/*.hpp)
FILE( GLOB top_impl yocto/*.cpp)
SET( top_srcs ${top_decl} ${top_impl} )
INSTALL( FILES ${top_decl} DESTINATION include/yocto )

FILE( GLOB code_decl yocto/code/*.hpp)
FILE( GLOB code_impl yocto/code/*.cpp)
SET( code_srcs ${code_decl} ${code_impl})
INSTALL( FILES ${code_decl} DESTINATION include/yocto/code )
SOURCE_GROUP( code FILES ${code_srcs} )

FILE( GLOB threading_decl yocto/threading/*.hpp)
FILE( GLOB threading_impl yocto/threading/*.cpp)
SET( threading_srcs ${threading_decl} ${threading_impl} )
INSTALL( FILES ${threading_decl} DESTINATION include/yocto/threading)
SOURCE_GROUP(threading FILES ${threading_srcs} )

FILE( GLOB core_decl yocto/core/*.hpp)
FILE( GLOB core_impl yocto/core/*.cpp)
SET( core_srcs ${core_decl} ${core_impl})
SOURCE_GROUP( core FILES ${core_srcs} )

FILE( GLOB mem_decl yocto/memory/*.hpp)
FILE( GLOB mem_impl yocto/memory/*.cpp)
SET( mem_srcs ${mem_decl} ${mem_impl} )
INSTALL( FILES ${mem_decl} DESTINATION include/yocto/memory )
SOURCE_GROUP( memory FILES ${mem_srcs} )


SET( decl ${top_decl} ${code_decl} ${threading_decl} ${core_decl} ${mem_decl})
SET( srcs ${top_srcs} ${code_srcs} ${threading_srcs} ${core_srcs} ${mem_srcs})

INCLUDE_DIRECTORIES( . )

SET( utest_decl yocto/utest/driver.hpp yocto/utest/run.hpp )
SOURCE_GROUP( utest FILES ${utest_decl})
ADD_LIBRARY(yocto STATIC ${srcs} ${utest_decl})

INSTALL( TARGETS yocto ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

