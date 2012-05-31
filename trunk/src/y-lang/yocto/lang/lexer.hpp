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
        
        //! convert a regex::source into a lexeme source
        /**
         use multiple scanner
         */
        class lexer 
        {
        public:
            explicit lexer( const string &main_id );
            explicit lexer( const char   *main_id );
            virtual ~lexer() throw();
            
            lexical::scanner & first() throw();
            
            lexical::scanner & declare( const string &id );
            lexical::scanner & declare( const char   *id );
            
            void jump( const string &id ); //!< change current, no history record
            void call( const string &id ); //!< change current, with history record 
            void back();                   //!< change current to top of history
            void reset() throw();          //!< restart lexer
            
            lexeme *next_lexeme(regex::source &src); //! get next lexeme
            const lexical::scanner &current() const throw();
            
            //! put in cache an artificial lexeme with label from.name
            void  unget( const lexical::scanner &from, const string &data );
            
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
