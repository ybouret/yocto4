#include "yocto/chemical/lua/io.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace  chemical
    {
        
        namespace
        {
            
            class LuaEffector : public effector
            {
            public:
                explicit LuaEffector(const string &id, lua_State *vm) :
                effector(id),
                L(vm)
                {
                    assert(L!=0);
                    const char *ID = name.c_str();
                    lua_getglobal(L,ID);
                    if(!lua_isfunction(L, -1))
                        throw exception("effector '%s': no function",ID);
                    lua_pop(L,1);
                }
                
                virtual ~LuaEffector() throw()
                {
                }
                
                virtual
                void rate(array<double>       &rho,
                          const double         t,
                          const array<double> &Cin,
                          const array<double> &Cout,
                          const parameters    &params)
                {
                    std::cerr << "Calling LuaEffector<" << name << ">" << std::endl;
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(LuaEffector);
                lua_State *L;
            };
            
        }
        
        void __lua:: load( lua_State *L, effectors &edb, const string &id )
        {
            assert(L);
            lua_settop(L,0);
            const char *ID = id.c_str();
            lua_getglobal(L,ID);
            if( ! lua_istable(L,-1) )
            {
                throw exception("effectors '%s': not a LUA_TABLE", ID);
            }
            
            const size_t n = lua_rawlen(L, -1);
            for(size_t i=1;i<=n;++i)
            {
                // get the name
                lua_rawgeti(L, -1, i);
                if( !lua_isstring(L,-1) )
                {
                    throw exception("effectors '%s'[%u] is not a LUA_STRING", ID, unsigned(i) );
                }
                
                const string            name = lua_tostring(L, -1);
                std::cerr << "\t(+) <" << name << ">" << std::endl;
                const effector::pointer p( new LuaEffector(name,L) );
                if(! edb.insert(p) )
                {
                    throw exception("effectors '%s': multiple '%s'", ID, name.c_str());
                }
                
                // remove the id
                lua_pop(L,1);
            }
        }

    }

}
