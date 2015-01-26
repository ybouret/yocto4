#ifndef YOCTO_RTLD_INTERFACE_INCLUDED
#define YOCTO_RTLD_INTERFACE_INCLUDED 1

#include "yocto/rtld/module.hpp"
#include "yocto/rtld/export.hpp"
#include "yocto/exception.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/pooled.hpp"

#include <typeinfo>
#include <cstring>

namespace yocto
{


    // common shared content of an interface
    class rtld_content : public counted_object
    {
    public:
        static const char   loader_name[];
        static const size_t max_message_length = 80;
        typedef memory::buffer_of<char,memory::pooled> message_type;

        virtual ~rtld_content() throw();

        const char  *uid;  //!< api name from types
        message_type msg;  //!< to get i/o error

        bool is_loaded() const throw();
        
    protected:
        explicit rtld_content( const string &soname, const char *api_name);
        dylib   *dll;

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(rtld_content);


    };

    //! interface to a C-API
    template <typename C_API>
    class interface
    {
    public:
        static const size_t num_entries = sizeof(C_API)/sizeof(void*);

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
                if(dll)
                {
                    void (YOCTO_API *ld)(const C_API *) = 0; // loading function
                    const module M(dll);
                    M.link(ld,loader_name);                // fetch it in the dll
                    if( !ld )
                        throw exception("plugin<%s>(no loader '%s')", uid, loader_name );
                    ld( &api );                              // populate API
                }
            }

            inline void clear() throw() { memset( (void*)&api,0,sizeof(C_API)); }
        };



        inline interface( const string &soname ) :
        handle(  content::create(soname) )
        {
            handle->withhold();
        }

        inline interface( char *soname ) :
        handle( content::create(soname)  )
        {
            handle->withhold();
        }

        inline interface(const interface &I) throw() : handle( I.handle )
        {
            handle->withhold();
        }
        
        virtual ~interface() throw()
        {
            if( handle->liberate() )
            {
                delete handle;
            }
            handle = 0;
        }

        inline bool          is_loaded()  const throw() { return handle->is_loaded(); }
        inline const char *  when()       const throw() { return handle->msg();       }
        inline const C_API * operator->() const throw() { return & (handle->api);     }
        inline const char  * name()       const throw() { return handle->uid;         }
        
    private:
        content *handle;
        YOCTO_DISABLE_ASSIGN(interface);
        
    };

#define YOCTO_INTERFACE_CHECK(I,ENTRY) do { if(0==I->ENTRY) throw exception("'%s': missing '%s'", I.name(), #ENTRY); } while(false)

}


#endif
