#include "yocto/lua/x/lua-vec3.hpp"

namespace yocto {

	namespace Lua {

		//-- IMPLEMENTATION
		YOCTO_LUA_IMPL(vec3);


		//-- GETTERS
		YOCTO_LUA_GETTERS_OF(vec3)
		{
			YOCTO_LUA_GET(x,lua_Number,vec3,vtx.x),
			YOCTO_LUA_GET(y,lua_Number,vec3,vtx.y),
			YOCTO_LUA_GET(z,lua_Number,vec3,vtx.z),
			YOCTO_LUA_GET_DONE()
		};

		//-- SETTERS
		YOCTO_LUA_SETTERS_OF(vec3)
		{
			YOCTO_LUA_SET(x,lua_Number,vec3,vtx.x),
			YOCTO_LUA_SET(y,lua_Number,vec3,vtx.y),
			YOCTO_LUA_SET(z,lua_Number,vec3,vtx.z),
			YOCTO_LUA_SET_DONE()
		};

		//-- METHODS
		YOCTO_LUA_METHODS_OF(vec3)
		{
			YOCTO_LUA_METHOD(vec3,length),
			YOCTO_LUA_METHOD(vec3,size),
			YOCTO_LUA_METHOD_DONE()
		};

		//-- FUNCTIONS
		YOCTO_LUA_FUNCTIONS_OF(vec3)
		{
			YOCTO_LUA_FUNCTION(vec3,version),
			YOCTO_LUA_FUNCTION_DONE()
		};

		//-- EVENTS
		YOCTO_LUA_EVENTS_OF(vec3)
		{
			YOCTO_LUA_EVENT(vec3,tostring),
			YOCTO_LUA_EVENT(vec3,add),
			YOCTO_LUA_EVENT(vec3,sub),
			YOCTO_LUA_EVENT(vec3,unm),
			YOCTO_LUA_EVENT(vec3,pow),
			YOCTO_LUA_EVENT(vec3,mul),
			YOCTO_LUA_EVENT_DONE()
		};

	}

}
