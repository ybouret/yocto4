#include "yocto/lua/lua-state.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>
#include <cstdlib>

namespace yocto {
	
	namespace Lua {
		
		//! Lua Custom Memory Allocator => Thread Safe
		static
		void *VM_alloc( void *, void *ptr, size_t osize, size_t nsize ) throw() {
			(void)osize;
			YOCTO_GIANT_LOCK();
			if (nsize == 0) {
				free(ptr);
				return NULL;
			}
			else
				return realloc(ptr, nsize);
		}
		
		State:: State( ) :
		L( lua_newstate( VM_alloc, NULL ) )
		{
			if( !L )
				throw libc::exception( ENOMEM, "lua_newstate" );
			luaL_openlibs(L);
		}
		
		
		State:: ~State() throw() {
			lua_close(L);
		}
		
		
		lua_State * State:: operator()( void ) throw() {
			return L;
		}
		
		void DumpStack( lua_State *L ) {
			printf("<lua_State top='%d'>\n", lua_gettop(L) );
			for( int i=lua_gettop(L);i>0;--i) {
				printf("\t%d\t<%s>\n", i, luaL_typename(L,i));
			}
			printf("</lua_State>\n");
		}
		
		void DumpTable( lua_State *L, int t, const char *name ) {
			const char *tab = name ? name : "TABLE";
			printf("<%s>\n",tab);
			luaL_checktype( L, t, LUA_TTABLE);
			lua_pushnil(L);  /* first key */
			while (lua_next(L, t) != 0) {
				/* uses 'key' (at index -2) and 'value' (at index -1) */
				printf("\t%s - %s\n",
				       lua_typename(L, lua_type(L, -2)),
				       lua_typename(L, lua_type(L, -1)));
				/* removes 'value'; keeps 'key' for next iteration */
				lua_pop(L, 1);
			}
			printf("</%s>\n", name);
		}
		
		
		
	} // lua
	
}
