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
            
            const lexical::scanner & operator[](const string &id) const;
            const lexical::scanner & operator[](const char   *id) const;
            
            bool has( const string &id) const throw();    //!< check
            
            void initialize() throw(); //!< line=1, scan=root, reset scanners and history
            
            void jump( const string &id ); //!< change current scanner
            void call( const string &id ); //!< change current scanner, push old on history
            void back();                   //!< change current scanner with last of history
            
            //! find next lexeme
            lexeme *get( source &, ios::istream &);
            
            //! back into cache
            void    unget( lexeme *lx ) throw();
            
            //! back into cache, using the scanner name
            void    unget( const lexical::scanner &parent, const string &content);
            
            const lexeme *peek(source &,ios::istream&);
            
            //! information
            const lexical::scanner &current() const throw();
            
            
        private:
            typedef set<string,lexical::scanner::pointer> scanDB;
            typedef list<lexical::scanner*> history_type;
            lexical::scanner *scan;
            lexemes           cache;
            history_type      history;
            
        public:
            string last_label; //!< saved after a NEW lexeme
            string last_token; //!< saved after a NEW lexeme
            
        private:
            scanDB            scanners;
            lexical::scanner *root;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            
        public:
            p_dict dict;
            const lexical::action forward;
            const lexical::action discard;
            const lexical::action newline;
            
            bool emit(const token&) throw();
            bool drop(const token&) throw();
            void on_newline() throw();      //!< ++line
            bool on_newline_drop(const token&) throw(); //!< newline, return false
            bool on_newline_emit(const token&) throw(); //!< newline, return true
        };
        
    }
}

#endif


