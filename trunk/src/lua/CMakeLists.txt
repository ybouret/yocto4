PROJECT(lua C)
INCLUDE_DIRECTORIES(../main .)

SET(lua_root yocto/lua)

SET(decl
${lua_root}/lapi.h
${lua_root}/lauxlib.h
${lua_root}/lcode.h
${lua_root}/lctype.h
${lua_root}/ldebug.h
${lua_root}/ldo.h
${lua_root}/lfunc.h
${lua_root}/lgc.h
${lua_root}/llex.h
${lua_root}/llimits.h
${lua_root}/lmem.h
${lua_root}/lobject.h
${lua_root}/lopcodes.h
${lua_root}/lparser.h
${lua_root}/lstate.h
${lua_root}/lstring.h
${lua_root}/ltable.h
${lua_root}/ltm.h
${lua_root}/lua.h
${lua_root}/lua.hpp
${lua_root}/luaconf.h
${lua_root}/lualib.h
${lua_root}/lundump.h
${lua_root}/lvm.h
${lua_root}/lzio.h
)

SET(impl
${lua_root}/lapi.c
${lua_root}/lauxlib.c
${lua_root}/lbaselib.c
${lua_root}/lbitlib.c
${lua_root}/lcode.c
${lua_root}/lcorolib.c
${lua_root}/lctype.c
${lua_root}/ldblib.c
${lua_root}/ldebug.c
${lua_root}/ldo.c
${lua_root}/ldump.c
${lua_root}/lfunc.c
${lua_root}/lgc.c
${lua_root}/linit.c
${lua_root}/liolib.c
${lua_root}/llex.c
${lua_root}/lmathlib.c
${lua_root}/lmem.c
${lua_root}/loadlib.c
${lua_root}/lobject.c
${lua_root}/lopcodes.c
${lua_root}/loslib.c
${lua_root}/lparser.c
${lua_root}/lstate.c
${lua_root}/lstring.c
${lua_root}/lstrlib.c
${lua_root}/ltable.c
${lua_root}/ltablib.c
${lua_root}/ltm.c
${lua_root}/lundump.c
${lua_root}/lvm.c
${lua_root}/lzio.c
)

SET(public_decl
${lua_root}/lua.h
${lua_root}/luaconf.h
${lua_root}/lualib.h
${lua_root}/lauxlib.h
${lua_root}/lua.hpp
)

ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS=1)
ADD_LIBRARY(y-lua STATIC ${decl} ${impl})
INSTALL( FILES   ${public_decl}  DESTINATION include/yocto/lua )
INSTALL( TARGETS y-lua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

#ADD_EXECUTABLE(lua lua.c)

#TARGET_LINK_LIBRARIES(lua y-lua)
#IF(MSVC)

#ELSE(MSVC)
	#TARGET_LINK_LIBRARIES(lua m)
#ENDIF(MSVC)

