#ifndef YOCTO_SWAMP_FIELD_LAYOUT_INCLUDED 
#define YOCTO_SWAMP_FIELD_LAYOUT_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/string.hpp"
#include "yocto/type-spec.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        class field_info
        {
        public:
            field_info(const char          *id, 
                       const std::type_info &which, 
                       const bool            asynchronous );
            ~field_info() throw();
            field_info( const field_info &other  ) ;            
            
            const string    name;
            const type_spec spec;
            const bool      async;
            
        private:
            YOCTO_DISABLE_ASSIGN(field_info);
        };
        
        class field_layout : public vector<field_info>
        {
        public:
            explicit field_layout() throw();
            virtual ~field_layout() throw();
            field_layout( const field_layout &other);
            template <typename ARRAY>
            inline void add( const char *name, bool async = true )
            {
                const field_info f(name, typeid(ARRAY), async);
                push_back( f );
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(field_layout);
        };
        
        
    }
}

#endif
