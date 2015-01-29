#include "yocto/chemical/lua/io.hpp"

namespace yocto
{
    namespace chemical
    {
        
        __lua::Library:: Library( lua_State *L, const char *id ) :
        library()
        {
            //__lua::register_functions(L);
            __lua::load(L,*this,id);
        }
        
        __lua::Library:: ~Library() throw() {}
        
    }
}

namespace yocto
{
    namespace chemical
    {
        __lua::Equilibria:: Equilibria( lua_State *L, const char *id, const library &lib ) :
        equilibria()
        {
            __lua::load(L,*this,id,lib);
            compile_for(lib);
        }
        
        __lua::Equilibria:: ~Equilibria() throw() {}
    }
    
    

}

namespace yocto
{
    namespace chemical
    {

        __lua:: Effectors:: Effectors(lua_State *L, const char *id) :
        effectors()
        {
            __lua::load(L,*this,id);
        }
        
        
        __lua:: Effectors:: ~Effectors() throw()
        {
            
        }
        
    }

}
