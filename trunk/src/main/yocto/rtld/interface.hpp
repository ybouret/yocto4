#ifndef YOCTO_RTLD_PLUGIN_INCLUDED
#define YOCTO_RTLD_PLUGIN_INCLUDED 1

#include "yocto/rtld/module.hpp"
#include "yocto/rtld/export.hpp"
#include "yocto/exception.hpp"
#include "yocto/counted.hpp"

#include <typeinfo>
#include <cstring>

namespace yocto
{
    
    //! interface to a C-API
    template <typename C_API>
    class interface
    {
    public:
        
        
        //! holds information: API/DLL
        class content : public object, public counted
        {
        public:
            
            virtual ~content() throw() { clear(); }
            
            const C_API api;
            const char *uid;
            
            static inline
            content * create( const module &m, const string &ldname )
            {
                return new content(m,ldname);
            }
            
            static inline
            content * create( const module &m, const char *ldname )
            {
                const string tmp(ldname);
                return new content(m,tmp);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(content);
            module       dll;
            
            explicit content( const module &m, const string &ldname ) :
            api(),
            uid(0),
            dll(m)
            {
                uid = typeid(C_API).name();
                clear();
                void (YOCTO_API *ld)(const C_API *) = 0; // loading function
                dll.link(ld,ldname);                     // fetch it in the dll
                if( !ld )
                    throw exception("plugin<%s>(no loader '%s')", uid, ldname.c_str() );
                ld( &api );                              // populate API                
            }
            
            inline void clear() throw() { memset( (void*)&api,0,sizeof(C_API)); }
        };
        
        
        //! dedicated reference counted smart pointer
        class pointer
        {
        public:
            inline pointer(const module &m, const string &ldname) :
            ld( content::create(m,ldname) ) { ld->withhold(); }
            
            inline pointer(const module &m, const char *ldname) :
            ld( content::create(m,ldname) ) { ld->withhold(); }
            
            inline pointer( const pointer &other ) throw() :
            ld( other.ld ) { ld->withhold(); }
            
            virtual ~pointer() throw()
            {
                assert(ld);
                if( ld->liberate() )
                {
                    delete ld;
                    ld = 0;
                }
            }
            
            inline const C_API   * operator->() const throw() { return &(ld->api); }
            inline const content & operator*()  const throw() { return *ld; }
            
        private:
            YOCTO_DISABLE_ASSIGN(pointer);
            content *ld;
            
        };
        
        const pointer handle;
        
        explicit interface( const module &m, const string &ldname) :
        handle( m, ldname )
        {
        }
        
        explicit interface( const module &m, const char *ldname ) :
        handle( m, ldname )
        {
            
        }
        
        virtual ~interface() throw() {}
        
        //! use "->" transitivity
        const C_API * operator->() const throw() { return &( (*handle).api ); }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(interface);
        
    };
    
}


#endif
