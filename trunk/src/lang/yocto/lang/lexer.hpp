#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"

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
            
            lexical::scanner & declare(const string &id); //!< a new scanner
            lexical::scanner & declare(const char   *id); //!< a new scanner
            bool has( const string &id) const throw();    //!< check
            
            void initialize() throw(); //!< line=1, scan=root, reset scanners and history
            
            void jump( const string &id ); //!< change current scanner
            void call( const string &id ); //!< change current scanner, push old on history
            void back();                   //!< change current scanner with last of history
            
            //! find next lexeme
            lexeme *get( source &src, ios::istream &);
           
            //! back into cache
            void    unget( lexeme *lx ) throw();

            //! information
            const lexical::scanner &current() const throw();
            
            
        private:
            typedef set<string,lexical::scanner::pointer> scanDB;
            typedef list<lexical::scanner*> history_type;
            lexical::scanner *scan;
            lexemes           cache;
            history_type      history;
            scanDB            scanners;
            lexical::scanner *root;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
    }
}

#endif


