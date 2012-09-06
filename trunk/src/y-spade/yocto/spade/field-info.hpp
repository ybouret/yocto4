#ifndef YOCTO_SPADE_FIELD_INFO_INCLUDED
#define YOCTO_SPADE_FIELD_INFO_INCLUDED 1

#include "yocto/type-spec.hpp"
#include "yocto/string.hpp"
#include "yocto/spade/linear.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <typename LAYOUT>
        class field_info
        {
        public:
            typedef void * (*array_ctor)( const LAYOUT &, linear **);
            typedef void   (*array_dtor)(void*);
            
            const string     name;
            const type_spec  spec;
            const array_ctor ctor;
            const array_dtor dtor;
                     
            inline field_info(const char           *a_name,
                              const std::type_info &a_spec,
                              const array_ctor      a_ctor,
                              const array_dtor      a_dtor) :
            name( a_name ),
            spec( a_spec ),
            ctor( a_ctor ),
            dtor( a_dtor )
            {
                assert(ctor);assert(dtor);
            }

            
            
            inline ~field_info() throw() {}
            
            inline field_info( const field_info &other ) :
            name(other.name),
            spec(other.spec),
            ctor(other.ctor),
            dtor(other.dtor)
            {
            }
            
            
            
        private:
            YOCTO_DISABLE_ASSIGN(field_info);
        };
        
    }
}

#endif
