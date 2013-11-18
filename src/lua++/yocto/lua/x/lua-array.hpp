#ifndef YOCTO_LUA_ARRAY_INCLUDED
#define YOCTO_LUA_ARRAY_INCLUDED 1

#include "yocto/lua/lua-build.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto {
	
	namespace Lua {
		
		typedef vector<lua_Number> ARRAY;
		
		YOCTO_LUA_DECL(array)
		{
		public:
			explicit array( lua_State *L );
			virtual ~array() throw();
			
			// methodes
			int size( lua_State *L);
			int capacity( lua_State *L);
			
			 //-- special access
			virtual int __rawget( lua_State *L ) {
				size_t index = luaL_checkint(L,1);
				assert(index>0);
				assert(index<=array_.size());
				lua_pushnumber(L,array_[index]);
				return 1;
			}

			virtual int __rawset( lua_State *L ) {
				size_t index = luaL_checkint(L,1);
				assert(index>0);
				assert(index<=array_.size());
				array_[index] = luaL_checknumber(L,2);
				return 0;
			}

			//-- functions
			
			//-- events
			static int __tostring( lua_State *L ); //!< scilab

			
			YOCTO_LUA_DONE(array);
		private:
			ARRAY array_;
		};
		
	}
	
}

#endif
