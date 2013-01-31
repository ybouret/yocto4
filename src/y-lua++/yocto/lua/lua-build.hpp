#ifndef _YOCTO_LUA_BUILD_HPP_
#define _YOCTO_LUA_BUILD_HPP_ 1


#include "yocto/lua/lua-object.hpp"

namespace yocto {

	namespace Lua
	{

		//======================================================================
		//
		// macros to declare an object
		//
		//======================================================================
		#define YOCTO_LUA_DECL(CLASS) \
		class CLASS : public Lua::Object


		#define YOCTO_LUA_DONE(CLASS) \
			public:\
			static const char ClassName[];   /**! for public lua API */\
			static const char PackageName[]; /** for functions table */\
			\
			static const Link::Member Getters[]; /**! expose variables to get */ \
			static const Link::Member Setters[]; /**! expose variables to set */ \
			\
			static const luaL_Reg     Functions[]; /**! static methods <=> package function */ \
			static const luaL_Reg     Events[];    /**! change semantic behavior */            \
			\
			typedef int (CLASS:: *Proc)( lua_State * );            /**!< lua callable method */ \
			struct Method { const char *Name; CLASS::Proc Addr; }; /**!< method to link      */ \
			static const CLASS::Method Methods[];                  /**!< OO API              */ \
			\
			virtual inline const char *GetClassName() const throw() \
			{ return CLASS::ClassName; }\
		private:\
			YOCTO_DISABLE_COPY_AND_ASSIGN(CLASS)

		//======================================================================
		// macros to implement an object
		//======================================================================
		#define YOCTO_LUA_IMPL(CLASS) \
		const char CLASS:: ClassName[]   = #CLASS; \
		const char CLASS:: PackageName[] = #CLASS "L"

		#define YOCTO_LUA_METHODS_OF(CLASS)   \
		const CLASS::Method CLASS::Methods[] =

		#define YOCTO_LUA_METHOD(CLASS,NAME) { #NAME, &CLASS::NAME }

		#define YOCTO_LUA_METHOD_DONE() { NULL, NULL}

		//----------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------
		#define YOCTO_LUA_GETTERS_OF(CLASS)      \
			const Lua::Link::Member              \
			CLASS:: Getters[] =

		#define YOCTO_LUA_GET_DONE() { NULL, NULL, 0 }


		//----------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------
		#define YOCTO_LUA_SETTERS_OF(CLASS) \
			const Lua::Link::Member \
			CLASS:: Setters[] =


		#define YOCTO_LUA_SET_DONE() { NULL, NULL, 0 }

		//----------------------------------------------------------------------
		// Static Methods = Functions
		//----------------------------------------------------------------------
		#define YOCTO_LUA_FUNCTIONS_OF(CLASS)  \
			const luaL_Reg                     \
			CLASS ::Functions[] =

		#define YOCTO_LUA_FUNCTION(CLASS,FUNCTION) \
			{ #FUNCTION, CLASS::FUNCTION }

		#define YOCTO_LUA_FUNCTION_DONE() \
			{ NULL, NULL }


		//----------------------------------------------------------------------
		// BuiltIn Events = Events
		//----------------------------------------------------------------------
		#define YOCTO_LUA_EVENTS_OF(CLASS) \
			const luaL_Reg                 \
			CLASS::Events[] =

		#define YOCTO_LUA_EVENT(CLASS,NAME) \
			{ "__" #NAME, CLASS:: __##NAME }

		#define YOCTO_LUA_EVENT_DONE() \
			{ NULL, NULL }

		//======================================================================
		//
		// templated creators
		//
		//======================================================================
		//! New C++ object from the lua stack
		/**
			the stack must contains initializers for the object
		*/
		template <class OBJECT>
		inline
		int New( lua_State *L ) {
			OBJECT  *pObj  = new OBJECT(L);
			OBJECT **ppObj = (OBJECT **)lua_newuserdata(L,sizeof(OBJECT *));
			*ppObj = pObj;
			static_cast<Lua::Object *>(pObj)->SetClassName(L);
			return 1;
		}

		//! New C++ object from the user world
		/**
			once the object is ready, the function that uses it MUST
			return 1 (+new objects on the stack, should be 0)
		*/
		template
		<
		class OBJECT,
		class SOURCE
		>
		inline
		OBJECT *New( lua_State *L, const SOURCE &S ) {
			OBJECT  *pObj  = new OBJECT(S);
			OBJECT **ppObj = (OBJECT **)lua_newuserdata(L,sizeof(OBJECT *));
			*ppObj = pObj;
			static_cast<Lua::Object *>(pObj)->SetClassName(L);
			return pObj;
		}


		//======================================================================
		//
		// templated cast
		//
		//======================================================================
		//! fast cast from lua_State[index] to OBJECT
		template <class OBJECT>
		inline
		OBJECT *Cast( lua_State *L, int index ) {
			OBJECT **ppObj = (OBJECT **)lua_touserdata(L,index);
			assert( NULL != ppObj );
			return *ppObj;
		}


		//! safe cast from lua_State[index] to OBJECT
		template <class OBJECT>
		inline
		OBJECT *Check( lua_State *L, int index ) {
			OBJECT **ppObj = (OBJECT **)luaL_checkudata(L,index,OBJECT::ClassName);
			if( NULL == ppObj ) {
                const char *req_type = OBJECT::ClassName;
                const char *usr_type = luaL_typename(L, index);
				luaL_error(L, "@index %d: invalide type '%s', expecting '%s'", index, usr_type, req_type);
			}
			return *ppObj;
		}

		//======================================================================
		//
		// templated destructor
		//
		//======================================================================
		template <class OBJECT>
		inline int Delete( lua_State *L )
		{
			#if defined(NDEBUG)
			OBJECT *obj = Lua::Cast<OBJECT>(L,1);
			#else
			OBJECT *obj = Lua::Check<OBJECT>(L,1);
			#endif
			delete obj;
			return 0;
		}

	}

}

#endif
