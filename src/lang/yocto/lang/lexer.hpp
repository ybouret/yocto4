#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"

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
            
            lexical::scanner & declare(const string &id);
            lexical::scanner & declare(const char   *id);
            
        private:
            typedef set<string,lexical::scanner::pointer> scanDB;
            scanDB            scanners;
            lexical::scanner *root;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
    }
}

#endif


