#ifndef YOCTO_RTLD_INTERFACE_INCLUDED
#define YOCTO_RTLD_INTERFACE_INCLUDED 1

#include "yocto/rtld/module.hpp"
#include "yocto/rtld/export.hpp"
#include "yocto/exception.hpp"
#include "yocto/counted-object.hpp"

#include <typeinfo>
#include <cstring>

namespace yocto
{


    // common shared content of an interface
    class rtld_content : public counted_object
    {
    public:
        static const char loader_name[];

        virtual ~rtld_content() throw();

        const char *uid;

    protected:
        explicit rtld_content( const string &soname, const char *api_name);

        const module dll;


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(rtld_content);
    };

    //! interface to a C-API
    template <typename C_API>
    class interface
    {
    public:


        //! holds information: API/DLL
        class content : public rtld_content
        {
        public:

            inline virtual ~content() throw() { clear(); }

            const C_API api;

            static inline
            content * create( const string &soname )
            {
                return new content(soname);
            }

            static inline
            content * create( const char *soname )
            {
                const string tmp(soname);
                return new content(tmp);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(content);
            
            inline content( const string &soname ) :
            rtld_content(soname,typeid(C_API).name()),
            api()
            {
                clear();
                void (YOCTO_API *ld)(const C_API *) = 0; // loading function
                dll.link(ld,loader_name);                // fetch it in the dll
                if( !ld )
                    throw exception("plugin<%s>(no loader '%s')", uid, loader_name );
                ld( &api );                              // populate API
            }

            inline void clear() throw() { memset( (void*)&api,0,sizeof(C_API)); }
        };


        //! dedicated reference counted smart pointer
        class pointer
        {
        public:
            inline pointer(content *c) throw():
            ld( c ) { assert(ld); ld->withhold(); }


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

        inline interface( const string &soname ) :
        handle(  content::create(soname) )
        {
        }

        inline interface( char *soname ) :
        handle( content::create(soname)  )
        {
            
        }

        inline interface(const interface &I) throw() : handle( I.handle ) {}
        
        virtual ~interface() throw() {}
        
        //! use "->" transitivity
        const C_API * operator->() const throw() { return &( (*handle).api ); }
        
    private:
        YOCTO_DISABLE_ASSIGN(interface);
        
    };
    
}


#endif
