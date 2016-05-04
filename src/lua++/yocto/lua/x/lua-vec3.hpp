#ifndef YOCTO_LUA_X_V3D_INCLUDED
#define YOCTO_LUA_X_V3D_INCLUDED 1

#include "yocto/lua/lua-build.hpp"
#include "yocto/math/point3d.hpp"

namespace yocto {

	namespace Lua {

		typedef point3d<lua_Number> VEC3;

		//!
		/** \brief class vec3, host VEC3
		*/
		YOCTO_LUA_DECL(vec3)
		{
		public:
			explicit vec3( lua_State *L );
			explicit vec3( const VEC3 & ) throw();

			virtual ~vec3() throw();

			//-- methods
			int length(lua_State *L); //!< |vtx|
			int size( lua_State *L ); //!< returns 3


			virtual int __rawget( lua_State *L ); //!< [1], [2], [3]
			virtual int __rawset( lua_State *L ); //!< [1], [2], [3]


			//-- functions
			static int version( lua_State *L );


			//-- events
			static int __tostring( lua_State *L ); //!< scilab style
			static int __add( lua_State *L );      //!< lhs+rhs
			static int __sub( lua_State *L );      //!< lhs-rhs
			static int __unm( lua_State *L );      //!< -lhs
			static int __pow( lua_State *L );      //!< lhs cross rhs
			static int __mul( lua_State *L );      //!< lhs.rhs or scalar.rhs

			VEC3 vtx;

			YOCTO_LUA_DONE(vec3);
		};

	}


}

#endif
