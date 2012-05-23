#ifndef YOCTO_LEXICAL_SCANNER_INCLUDED
#define YOCTO_LEXICAL_SCANNER_INCLUDED 1

#include "yocto/lexical/unit.hpp"
#include "yocto/lexical/rule.hpp"

namespace yocto 
{
    namespace lexical
    {
        
        
        class scanner 
        {
        public:
            explicit scanner();
            virtual ~scanner() throw();
            
            size_t line;
            
        private:
            rules  rules_;
            units  cache_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
            bool   on_newline( const regex::token & ) throw();
            
        public:
            const action newline; //! default newline: ++line;
        };
        
        
    }
}

#endif
