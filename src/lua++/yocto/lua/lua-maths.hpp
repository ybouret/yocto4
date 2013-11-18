#ifndef _LUA_MATH_HPP_
#define _LUA_MATH_HPP_ 1

#include "yocto/lua/lua-state.hpp"

#include "yocto/string.hpp"
#include "yocto/functor.hpp"

namespace yocto {
	
	namespace Lua
	{
		
		template <typename T>
		class Function  : public object
		{
		public:
			explicit Function( lua_State *, const string &id, bool check=true );
			virtual ~Function() throw();
			Function( const Function & );
			Function & operator=( const Function & );
			
			T operator()( T );
			T operator()( T,T );

			
		private:
			lua_State    *L;
		public:
			const string  name;
			
		};
		
				
		
		
	}
	
}

#endif
