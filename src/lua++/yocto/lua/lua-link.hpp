#ifndef YOCTO_LUA_LINK_INCLUDED
#define YOCTO_LUA_LINK_INCLUDED 1


#include "yocto/string.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/core/offset.hpp"

namespace yocto {


	namespace Lua {

		//! C-Style member linking
		struct Link {
			static const int Index = 3;                      //!< index where the address is
			typedef int (*Function)( lua_State *L, void *v); //! get/set object at Link::Index

			struct Member {
				const char     *name;    //!< member name
				Link::Function  access;  //!< how to get it
				size_t          offset;  //!< where to find it
			};

			static void Expose( lua_State *L, const Link::Member *M ); //!< expose members into the current table
			static int  Call(lua_State *L);                            //!< perform the call to access.
		};

		template <class T>
		struct LinkAs
		{
			static int Set( lua_State *L, void *v ); //!< lua stack -> member
			static int Get( lua_State *L, void *v ); //!< member    -> lua stack
		};

		#if defined(__DMC__)
		template struct LinkAs<string>;
		template struct LinkAs<int>;
		template struct LinkAs<lua_Number>;
		#endif

		#define YOCTO_LUA_GET(LUA_NAME,MEMBER_TYPE,CLASS,MEMBER) \
		{ #LUA_NAME, LinkAs<MEMBER_TYPE>::Get, YOCTO_OFFSET_OF(CLASS,MEMBER) }


		#define YOCTO_LUA_SET(LUA_NAME,MEMBER_TYPE,CLASS,MEMBER) \
		{ #LUA_NAME, LinkAs<MEMBER_TYPE>::Set, YOCTO_OFFSET_OF(CLASS,MEMBER) }


		int __index_bind( lua_State *L );     //!< generic __index function.
		int __newindex_bind( lua_State *L );  //!< generic __new_index function.

	}

}

#endif
