#ifndef YOCTO_RTLD_PLUGIN_INCLUDED
#define YOCTO_RTLD_PLUGIN_INCLUDED 1

#include "yocto/rtld/module.hpp"
#include "yocto/rtld/export.hpp"
#include "yocto/exception.hpp"
#include "yocto/shared-ptr.hpp"
#include <typeinfo>
#include <cstring>

namespace yocto
{
    
    template <typename C_API>
    class plugin
    {
    public:
        class content : public object
        {
        public:
            explicit content( const module &m, const string &ldname ) :
            api(),
            uid(0),
            dll(m)
            {
                initialize(ldname);
            }
            
            virtual ~content() throw() {}
            
            const C_API api;
            const char *uid;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(content);
            module       dll;
            
            inline void clear() throw() { memset( (void*)&api,0,sizeof(C_API)); }
            
            inline void initialize(const string &ldname )
            {
                uid = typeid(C_API).name();
                clear();
                void (YOCTO_API *ld)(const C_API *) = 0; // loading function
                dll.link(ld,ldname);                     // fetch it in the dll
                if( !ld )
                {
                    // check
                    throw exception("plugin<%s>(no loader '%s')", uid, ldname.c_str() );
                }
                ld( &api );                              // populate API
            }
            
        };
        typedef shared_ptr<content> handle;
        const handle                h;
        
        explicit plugin( const module &m, const string &ldname) :
        h( new content(m,ldname) ) {}
        
        virtual ~plugin() throw() {}
        
        //! use "->" transitivity
        const C_API * operator->() const throw() { return &(h->api); }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
    };
    
    
}


#endif
