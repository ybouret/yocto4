#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/stock/stack.hpp"
#include "yocto/sequence/list.hpp"

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
            void call( const string &id );
            void reset() throw();
            
            lexeme *next_lexeme(regex::source &src);
            const lexical::scanner &current() const throw();
            
            size_t            line;
        private:
            typedef set<string,lexical::scanner::ptr> scannerDB;
            typedef lexical::scanner *                scan_ptr;
            typedef list<scan_ptr>                    hsequence;
            typedef stack<scan_ptr,hsequence>         historyDB;
            scannerDB         scanners;
            lexical::scanner *scan;
            lexemes           cache;
            historyDB         history;
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            lexical::scanner *fetch( const string &id ) const;
            lexical::scanner *fetch( const char   *id ) const;
        };
        
    }
    
}

#endif
