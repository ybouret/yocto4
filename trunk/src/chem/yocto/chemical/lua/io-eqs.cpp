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
                F(L,fn,true)
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
            
        }
    }
    
}
