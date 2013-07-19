#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        class _lua:: equilibrium : public chemical::equilibrium
        {
        public:
            explicit equilibrium( const string &name, lua_State *vm, const string &fn ) :
            chemical::equilibrium(name),
            L(vm),
            F(fn)
            {
                assert(L);
            }
            
            virtual ~equilibrium() throw()
            {
            }
            
            
            virtual double getK( double t ) const
            {
                lua_getglobal( L, &F[0] );
                assert( lua_isfunction(L,-1) );
                lua_pushnumber(L,t); //-- push t onto the stack
                if( lua_pcall(L,1,1,0) )
                    throw exception( "%s", lua_tostring(L,-1) );
                const double ans = lua_tonumber(L,-1);
                lua_pop(L,1);  //-- the result
                return ans;
            }
            
            mutable lua_State *L;
            const   string     F;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
        };
        
        
        static inline
        void __parse_one_eq(lua_State *L,
                            const collection &lib,
                            equilibria &cs,
                            const char *table_name,
                            const unsigned table_indx)
        {
            if(!lua_istable(L, -1))
            {
                throw exception("%s[%u] is not a LUA_TABLE", table_name, table_indx);
            }
            
            const size_t n = lua_rawlen(L, -1);
            
            //------------------------------------------------------------------
            // get the name
            //------------------------------------------------------------------
            if(n<1)
                throw exception("%s[%u] has no name", table_name, table_indx);
            lua_rawgeti(L, -1, 1);
            if( !lua_isstring(L, -1) ) throw exception("%s[%u][1] is not a string", table_name, table_indx);
            const string name = lua_tostring(L, -1);
            lua_pop(L,1);
            std::cerr << "__parsing '" << name << "'" << std::endl;
            
            //------------------------------------------------------------------
            // get the constant
            //------------------------------------------------------------------
            if(n<2)
                throw exception("%s[%u] has no constant", table_name, table_indx);
            lua_rawgeti(L, -1, 2);
            switch( lua_type(L, -1) )
            {
                case LUA_TNUMBER:
                {
                    std::cerr << "\tCONSTANT" << std::endl;
                    const equilibrium::ptr p( new constant_equilibrium(name, lua_tonumber(L,-1)));
                    if(!cs.insert(p))
                    {
                        throw exception("multiple equilibrium '%s'", name.c_str());
                    }
                }
                    break;
                    
                case LUA_TSTRING:
                {
                    std::cerr << "\tFUNCTION" << std::endl;
                    const string fn = lua_tostring(L, -1);
                    lua_getglobal(L, fn.c_str());
                    if( !lua_isfunction(L, -1))
                        throw exception("%s is not a function", fn.c_str());
                    lua_pop(L, 1);
                    const equilibrium::ptr p( new _lua::equilibrium(name,L,fn) );
                    if(!cs.insert(p))
                    {
                        throw exception("multiple equilibrium '%s'", name.c_str());
                    }
                }
                    break;
                    
                default:
                    throw exception("%s[%u][2] is not a number or a string", table_name, table_indx);
            }
            lua_pop(L,1);
            
            
            //------------------------------------------------------------------
            // get the stochiometry
            //------------------------------------------------------------------
            equilibrium::ptr *ppEq = cs.search(name);
            if(!ppEq)
                throw exception("unexpected equilibria['%s'] failure", name.c_str());
            
            const char *eq_name = name.c_str();
            for( size_t i=3;i<=n;++i)
            {
                //--------------------------------------------------------------
                //-- push actor
                //--------------------------------------------------------------
                lua_rawgeti(L, -1, i);
                const unsigned ia = i-2;
                if(!lua_istable(L, -1))
                    throw exception("%s[%s] actor #%u is not a LUA_TABLE", table_name, eq_name, ia );
                if( lua_rawlen(L, -1) != 2 )
                    throw exception("%s[%s] actor #%u is not valid", table_name,eq_name,ia);
                
                //-- push factor
                lua_rawgeti(L,-1,1);
                if(!lua_isnumber(L, -1))
                    throw exception("%s[%s]  invalid stochiometric #%u", table_name, eq_name, ia);
                const int coef = int(lua_tonumber(L,-1));
                //-- pop factor
                lua_pop(L,1);
                
                //-- push name
                lua_rawgeti(L, -1, 2);
                if(!lua_isstring(L,-1))
                    throw exception("%s[%s] invalid name #%u", table_name, eq_name, ia);
                const string actor_name = lua_tostring(L, -1);
                //-- pop name
                lua_pop(L,1);
                
                (**ppEq).add( lib[actor_name], coef);
                
                //--------------------------------------------------------------
                //-- pop actor
                //--------------------------------------------------------------
                lua_pop(L, 1);
            }
            
        }
        
        void _lua:: load(lua_State *L, const collection &lib, equilibria &cs, const string &name)
        {
            assert(L);
            lua_settop(L, 0);
            const char *table_name = &name[0];
            lua_getglobal(L,table_name);
            if(!lua_istable(L, -1))
            {
                throw exception("%s is not a LUA_TABLE", table_name);
            }
            
            const size_t n = lua_rawlen(L, -1);
            
            for(size_t i=1;i<=n;++i)
            {
                //--------------------------------------------------------------
                // push the equilibrium
                //--------------------------------------------------------------
                lua_rawgeti(L, -1, i);
                
                __parse_one_eq(L, lib, cs, table_name, i);
                
                //--------------------------------------------------------------
                // pop the equilibrium
                //--------------------------------------------------------------
                lua_pop(L, 1);
            }
            
            
        }
        
        
        void _lua:: load( lua_State *L, const collection &lib, equilibria &cs, const char   *name )
        {
            const string NAME(name);
            _lua::load(L,lib,cs,NAME);
        }
    }
    
}
