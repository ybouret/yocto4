#ifndef YOCTO_RTLD_PLUGIN_INCLUDED
#define YOCTO_RTLD_PLUGIN_INCLUDED 1

#include "yocto/rtld/module.hpp"
#include "yocto/rtld/export.hpp"

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
            void YOCTO_API (*ld)(void *) = 0; // loading function
            dll.link(ld,ldname);              // fetch it in the dll
            ld( (void*)&api );                // populate API
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
