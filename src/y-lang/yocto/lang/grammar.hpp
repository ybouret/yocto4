#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/alternate.hpp"

namespace yocto 
{
    namespace lang
    {
        
        class grammar 
        {
        public:
            const string name;
            
            explicit grammar( const string &id );
            virtual ~grammar() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
        };
        
        
    }
    
}

#endif
