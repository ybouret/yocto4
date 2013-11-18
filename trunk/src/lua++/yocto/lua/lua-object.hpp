#ifndef YOCTO_LUA_OBJECT_INCLUDED
#define YOCTO_LUA_OBJECT_INCLUDED 1

#include "lua-link.hpp"

namespace yocto {

	namespace Lua {

		//! base C++ object for lua
		/**
			used to set metatable of the derived object (i.e. Lua thinks it's
			a new type )
		*/
		class Object : public object {
			public:
				virtual ~Object() throw();

				//==============================================================
				// user API name
				//==============================================================
				void                SetClassName( lua_State *L) const; //!< tag userdata with GetClassName
				virtual const char *GetClassName()  const throw() = 0; //!< for metatable tagging

				//==============================================================
				// special default methods
				//==============================================================
				virtual int __rawget( lua_State *L );//!< called upon get OBJECT[number]
				virtual int __rawset( lua_State *L );//!< called upon set OBJECT[number]


			protected:
				explicit Object() throw();

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Object);

		};


	}

}

#endif
