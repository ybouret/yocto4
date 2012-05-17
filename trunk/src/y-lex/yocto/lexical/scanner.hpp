#ifndef YOCTO_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lexical/unit.hpp"

namespace yocto 
{
    namespace lexical
    {
        
        
        class scanner 
        {
        public:
            explicit scanner() throw();
            virtual ~scanner() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
        };
        
        
    }
}

#endif
