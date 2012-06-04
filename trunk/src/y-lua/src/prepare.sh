# !bash
function xerror
{
    cmake -E cmake_echo_color --red "-- $1";
    exit 1;
}

################################################################################
## Fetch
################################################################################
LUA_DIR="lua-5.1.5"
LUA_TGZ="$LUA_DIR.tar.gz"
LUA_URL="http://www.lua.org/ftp/$LUA_TGZ"
cmake -E cmake_echo_color --blue "-- Downloading $LUA_URL"
curl $LUA_URL -o $LUA_TGZ || xerror "Could not Download !";

################################################################################
## Extract
################################################################################
cmake -E cmake_echo_color --blue "-- Extracting Archive";
tar xfz $LUA_TGZ || xerror "Could not Extract !";

################################################################################
## Public API
################################################################################
DST_DIR="../yocto/lua";
cmake -E cmake_echo_color --magenta "-- Copy Public Headers"
LUA_HEADERS="src/lua.h src/luaconf.h src/lualib.h src/lauxlib.h etc/lua.hpp";
for h in $LUA_HEADERS; do
    cmake -E cmake_echo_color --cyan --bold "--    $h";
    cmake -E copy_if_different $LUA_DIR/$h $DST_DIR/`basename $h` || xerror "Could not Copy !";
done;

################################################################################
## Private Headers
################################################################################
cmake -E cmake_echo_color --magenta "-- Copy Private Headers"
LUA_HELPERS="\
	lapi.h     \
	lcode.h    \
	ldebug.h   \
	ldo.h      \
	lfunc.h    \
	lgc.h      \
	llex.h     \
	llimits.h  \
	lmem.h     \
	lobject.h  \
	lopcodes.h \
	lparser.h  \
	lstate.h   \
	lstring.h  \
	ltable.h   \
	ltm.h      \
	lundump.h  \
	lvm.h      \
	lzio.h"

for h in $LUA_HELPERS; do
    cmake -E cmake_echo_color --cyan "--    $h";
    cmake -E copy_if_different $LUA_DIR/src/$h ./inc/$h || xerror "Could not Copy !";
done

################################################################################
## Make Lua in one File
################################################################################
LUA_ALL="$DST_DIR/lua-all.c"
LUA_SOURCES="\
	lapi.c \
	lcode.c \
	ldebug.c \
	ldo.c \
	ldump.c \
	lfunc.c \
	lgc.c \
	llex.c \
	lmem.c \
	lobject.c \
	lopcodes.c \
	lparser.c \
	lstate.c \
	lstring.c \
	ltable.c \
	ltm.c \
	lundump.c \
	lvm.c \
	lzio.c \
	lauxlib.c \
	lbaselib.c \
	ldblib.c \
	liolib.c \
	linit.c \
	lmathlib.c \
	loadlib.c \
	loslib.c \
	lstrlib.c \
	ltablib.c"

echo "#define luaall_c" > $LUA_ALL
echo "#if !defined(__STRICT_ANSI__)" >> $LUA_ALL
echo "#	define  __STRICT_ANSI__ 1" >> $LUA_ALL
echo "#endif" >> $LUA_ALL
cmake -E cmake_echo_color --magenta "-- Building lua-all"
for SRC in $LUA_SOURCES; do
	cmake -E cmake_echo_color --cyan "--    $SRC";
	cat  $LUA_DIR/src/$SRC >> $LUA_ALL;
	echo " "   >> $LUA_ALL;
done

cmake -E cmake_echo_color --blue "-- Standalone Lua is Build";
rm -Rf $LUA_DIR $LUA_TGZ
