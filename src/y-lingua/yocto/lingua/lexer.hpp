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
            explicit lexer(const string &id);
            virtual ~lexer() throw();
            
            const string name;
            size_t       line; //!< shared by scanners
            
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            set<string,lexical::scanner::ptr> scanners;
            
        };
        
    }
}

#endif
