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
            
            lexical::scanner & first() throw();
            
            lexical::scanner & declare( const string &id );
            lexical::scanner & declare( const char   *id );
            
            void jump( const string &id );
            void reset() throw();
            
            
            
        private:
            typedef set<string,lexical::scanner::ptr> scannerDB;
            size_t            line;
            scannerDB         scanners;
            lexical::scanner *current;
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            lexical::scanner *fetch( const string &id ) const;
            lexical::scanner *fetch( const char   *id ) const;
        };
        
    }
    
}

#endif
