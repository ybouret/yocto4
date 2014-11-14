#ifndef YOCTO_SPADE_VARIABLES_INCLUDED
#define YOCTO_SPADE_VARIABLES_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/memory/pooled.hpp"

namespace yocto
{
    namespace spade
    {
        
        typedef vector<string,memory::pooled::allocator> variables_base;
        
        class variables : public variables_base
        {
        public:
            explicit variables() throw();
            variables( const variables &other );
            virtual ~variables() throw();
            
            void append( const string &id );
            void append( const char   *id );
            
            variables & operator<<( const string &id );
            variables & operator<<( const char   *id );
            
        private:
            YOCTO_DISABLE_ASSIGN(variables);
        };
        
    }
}

#endif
