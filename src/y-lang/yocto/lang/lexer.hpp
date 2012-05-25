#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        
        class lexer 
        {
        public:
            explicit lexer( const string &main_id );
            virtual ~lexer() throw();
            
            lexical::scanner & declare( const string &id );
            lexical::scanner & declare( const char   *id );
            
        private:
            size_t line;
            set<string,lexical::scanner::ptr> scanners;
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
    }
    
}

#endif
