#ifndef YOCTO_LINGUA_LEXER_INCLUDED
#define YOCTO_LINGUA_LEXER_INCLUDED 1

#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //______________________________________________________________________
        //
        //! a source of lexemes
        //______________________________________________________________________
        class lexer : public virtual object
        {
        public:
            
            explicit lexer(const string &id);
            explicit lexer(const char   *id);
            virtual ~lexer() throw();
            
            const string name;
            size_t       line; //!< shared by scanners
            
            
            //! create a new scanner
            lexical::scanner & declare( const string &id );
            
            //! create a new scanner
            lexical::scanner & declare(const char *id);
            
            //! get the sub-scanner
            const lexical::scanner & sub(const string &id) const;
            
            //! get the sub-scanner
            const lexical::scanner & sub(const char   *id) const;
            
            //! line=1, reset all, clear history and cache
            void reset() throw();
            
            void jump( const string &id); //!< change current scanner
            void call( const string &id); //!< change current scanner, keeping track
            void back();                  //!< returns from call
            
            //! get the current scanner
            const lexical::scanner & current() const;
            
            //! get the first scanner
            const lexical::scanner & first() const;
            
            
            //! get next lexeme: NULL => EOF
            lexeme *get( source &src );
            
            //! back into cache
            void    unget( lexeme *lx ) throw();
            
            
            //! put in cache an artificial lexeme with label from.name
            /**
             a sub scanner can cache a lexeme to mimic
             a complex expression (aka real string).
             */
            void  emit( const lexical::scanner &from, const string &data );
            
            
            //! test if possible supplementary lexemes from source
            bool is_active( source &src );
            
            //! assume cache.size > 0
            const lexeme *peek() const throw();
            
            //! assume cache.size > 0
            const lexeme *last() const throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            typedef lexical::scanner *                scanner_ptr;
            typedef set<string,lexical::scanner::ptr> scannerDB;
            typedef list<scanner_ptr>                 history_t;
            
            lexical::scanner       *scan;       //!< current
            lexemes                 cache;      //!< cache
            scannerDB               scanners;   //!< database
            lexical::scanner       *init;       //!< main
            history_t               history;    //!< for call/back handling
            
            lexical::scanner *fetch( const string &id ) const throw();
        };
        
    }
}

#endif
