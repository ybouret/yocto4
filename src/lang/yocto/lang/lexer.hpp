#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"


namespace yocto
{
    namespace lang
    {

        //! transform an input into a source of lexemes
        class lexer
        {
        public:
            explicit lexer(const string &id);
            explicit lexer(const char   *id);
            virtual ~lexer() throw();
            
            const string name;
            int          line;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
    }
}

#endif


