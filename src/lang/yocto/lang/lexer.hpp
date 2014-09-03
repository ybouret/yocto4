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
            void set_root(const string &id);              //!< change root
            bool has( const string &id) const throw();    //!< check
            
            void initialize() throw(); //!< line=1, scan=root, reset scanners
            
            void jump( const string &id ); //!< change current scanner
            void call( const string &id ); //!< change current scanner, push old on history
            void back();                   //!< change current scanner with last of history
            
        private:
            typedef set<string,lexical::scanner::pointer> scanDB;
            typedef list<lexical::scanner*> history_type;
            lexical::scanner *scan;
            history_type      history;
            scanDB            scanners;
            lexical::scanner *root;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
    }
}

#endif


