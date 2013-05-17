#ifndef YOCTO_LINGUA_LEXER_INCLUDED
#define YOCTO_LINGUA_LEXER_INCLUDED 1

#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"

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
            
            
            //! create a new scanner
            lexical::scanner & declare( const string &id );
            
            
            //! line=1, reset all, clear history
            void reset() throw();
            
            void jump( const string &id); //!< change current scanner
            void call( const string &id); //!< change current scanner, keeping track
            void back();                  //!< returns from call
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            typedef lexical::scanner *                scanner_ptr;
            typedef set<string,lexical::scanner::ptr> scannerDB;
            typedef list<scanner_ptr>                 history_t;
            
            lexical::scanner       *scan;       //!< current
            scannerDB               scanners;   //!< database
            lexical::scanner       *init;       //!< main
            history_t               history;    //!< for call/back handling
            
            lexical::scanner *fetch( const string &id ) throw();
        };
        
    }
}

#endif
