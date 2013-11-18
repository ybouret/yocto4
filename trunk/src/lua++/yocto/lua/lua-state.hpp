#ifndef YOCTO_LUA_STATE_INCLUDED
#define YOCTO_LUA_STATE_INCLUDED 1

#include "yocto/object.hpp"
#include "yocto/lua/lua.hpp"

namespace yocto {


	namespace Lua {

		//! RAII for a lua_State
		class State : public  object {
		public:
			explicit State();          //!< call luaL_openlibs.
			virtual ~State() throw();  //!< free resources.

			lua_State * operator()(void) throw();

			
		private:
			lua_State *L;
			YOCTO_DISABLE_COPY_AND_ASSIGN(State);
		};

		void DumpStack( lua_State *L );
		void DumpTable( lua_State *L, int index , const char *name = NULL);

	}


}

#endif
