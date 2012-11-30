#ifndef YOCTO_RTLD_PLUGIN_INCLUDED
#define YOCTO_RTLD_PLUGIN_INCLUDED 1

#include "yocto/rtld/module.hpp"
#include "yocto/rtld/export.hpp"
#include "yocto/exception.hpp"

#include <cstring>

namespace yocto
{
    
    template <typename C_API>
    class plugin
    {
    public:
        explicit plugin( const module &m, const string &ldname) :
        api(), 
        dll(m)
        {
            clear();
            void (YOCTO_API *ld)(const C_API *) = 0; // loading function
            dll.link(ld,ldname);                     // fetch it in the dll
			if( !ld )                                // check
			throw exception("plugin(no loader '%s')", ldname.c_str() ); 
            ld( &api );                              // populate API
        }
        
        virtual ~plugin() throw() { clear(); }
        
        const C_API api;
        
    private:
        module      dll; //!< to keep a reference
        YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
        inline void clear() throw() { memset( (void*)&api,0,sizeof(C_API)); }
    };
    
}


#endif
