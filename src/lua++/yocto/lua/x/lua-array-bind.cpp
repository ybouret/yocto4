#include "yocto/lua/x/lua-array.hpp"

namespace yocto {
	
	namespace Lua {
		
		
		//-- IMPLEMENTATION
		YOCTO_LUA_IMPL(array);

		//-- GETTERS
		YOCTO_LUA_GETTERS_OF(array)
		{
			YOCTO_LUA_GET_DONE()
		};

		//-- SETTERS
		YOCTO_LUA_SETTERS_OF(array)
		{
			YOCTO_LUA_SET_DONE()
		};
		//-- METHODS
		YOCTO_LUA_METHODS_OF(array)
		{
			YOCTO_LUA_METHOD(array,size),
			YOCTO_LUA_METHOD(array,capacity),
			YOCTO_LUA_METHOD_DONE()
		};

		//-- FUNCTIONS
		YOCTO_LUA_FUNCTIONS_OF(array)
		{
			YOCTO_LUA_FUNCTION_DONE()
		};


		//-- EVENTS
		YOCTO_LUA_EVENTS_OF(array)
		{
			YOCTO_LUA_EVENT(array,tostring),
			YOCTO_LUA_EVENT_DONE()
		};

		
	}
	
}
