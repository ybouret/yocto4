#include "yocto/chemical/lua/io.hpp"
#include "yocto/exception.hpp"

#include "yocto/lua/lua-maths.hpp"

namespace yocto
{
    namespace chemical
    {
        
        namespace {
            
            class lua_equilibrium : public equilibrium
            {
            public:
                explicit lua_equilibrium(const string &id,
                                         lua_State    *L,
                                         const string &fn) :
                equilibrium(id),
                F(L,fn,false)
                {
                }
                
                virtual ~lua_equilibrium() throw()
                {
                }
                
                mutable Lua::Function<double> F;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(lua_equilibrium);
                virtual double getK(double t) const
                {
                    return F(t);
                }
            };
        
            
            static inline
            void __parse_actor(lua_State *L, equilibrium &eq, const unsigned j, const library &lib)
            {
                if(!lua_istable(L, -1))
                {
                    throw exception("%s actor #%u is not a LUA_TABLE",eq.name.c_str(),j);
                }
                
                // nu
                lua_rawgeti(L, -1, 1);
                if( !lua_isnumber(L, -1) )
                {
                    throw exception("%s actor #%u: nu is not a LUA_NUMBER", eq.name.c_str(),j);
                }
                const int nu = int(lua_tonumber(L, -1));
                lua_pop(L, 1);
                
                // name
                lua_rawgeti(L, -1, 2);
                if( !lua_isstring(L, -1) )
                {
                    throw exception("%s actor #%u: name is not a LUA_STRING", eq.name.c_str(),j);
                }
                const string name = lua_tostring(L,-1);
                lua_pop(L,1);
                
                // check
                if(!lib.has(name))
                {
                    throw exception("%s actor #%u: unknown species '%s'", eq.name.c_str(),j,name.c_str());
                }
                
                // add
                eq.add(lib[name], nu);
                
                
            }
            
            static inline
            void __parse_eq(lua_State *L, equilibria &eqs, const char *id, const library &lib, const unsigned i )
            {
                if(!lua_istable(L,-1))
                {
                    throw exception("%s[%u] is not a LUA_TABLE",id,i);
                }
                
                const size_t n = lua_rawlen(L, -1);
                //______________________________________________________________
                //
                // Get the name
                //______________________________________________________________
                lua_rawgeti(L,-1,1);
                if(!lua_isstring(L, -1))
                {
                    throw exception("%s[%u][1] is not a string (eq. name)",id,i);
                }
                const string name = lua_tostring(L, -1);
                lua_pop(L,1);
                
                //______________________________________________________________
                //
                // Get the constant, number or string
                //______________________________________________________________
                equilibrium *pEq = 0;
                lua_rawgeti(L,-1,2);
                if( lua_isnumber(L, -1) )
                {
                    pEq = new const_equilibrium(name,lua_tonumber(L, -1));
                    goto CHECK_EQ;
                }
                
                if( lua_isstring(L,-1) )
                {
                    const string fn = lua_tostring(L, -1);
                    pEq = new lua_equilibrium(name,L,fn);
                    goto CHECK_EQ;
                }
                
            CHECK_EQ:
                if(!pEq)
                {
                    throw exception("%s.%s: cannot make constant from data",id,name.c_str());
                }
                lua_pop(L,1);
                equilibrium &eq = eqs.add(pEq);
                
                //______________________________________________________________
                //
                // parse actors
                //______________________________________________________________
                for(size_t j=3;j<=n;++j)
                {
                    lua_rawgeti(L, -1, j);
                    __parse_actor(L,eq,j-2,lib);
                    lua_pop(L,1);
                }
                
                
            }
            
        }
        
        
        
        
        void __lua:: load(lua_State *L, equilibria &eqs,  const string &id , const library &lib)
        {
            assert(L);
            lua_settop(L,0);
            const char *txt = id.c_str();
            lua_getglobal(L,txt);
            if( !lua_istable(L,-1) )
            {
                throw exception("equilibria: '%s' is not a LUA_TABLE",txt);
            }
            
            const size_t n = lua_rawlen(L,-1);
            for(size_t i=1;i<=n;++i)
            {
                lua_rawgeti(L, -1, i);
                __parse_eq(L,eqs,txt,lib,i);
                lua_pop(L, 1);
            }
        }
        
        
    }
    
}
