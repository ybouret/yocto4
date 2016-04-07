#ifndef YOCTO_LINGUA_LEXER_INCLUDED
#define YOCTO_LINGUA_LEXER_INCLUDED 1

#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace lingua
    {

        class lexer : public object
        {
        public:
            virtual ~lexer() throw();
            explicit lexer();
            int    line;
            p_dict dict;

        private:
            typedef set<string,lexical::scanner::ptr> scanner_db;
            
            lexical::scanner *root;
            scanner_db        scdb;

            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
    }
}

#endif
