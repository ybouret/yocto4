#ifndef YOCTO_LANG_SCANNER_INCLUDED
#define YOCTO_LANG_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"

namespace yocto 
{
    namespace lang
    {
        
        namespace lexical
        {
            
            
            class scanner 
            {
            public:
                explicit scanner();
                virtual ~scanner() throw();
                
            private:
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
            };
        }
        
        
    }
}

#endif
