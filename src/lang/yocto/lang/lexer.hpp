#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace lang
    {
        
        //! transform an input into a source of lexemes
        /**
         may use mutliple dedicated scanners
         */
        class lexer : public virtual object
        {
        public:
            explicit lexer(const string &id);
            explicit lexer(const char   *id);
            virtual ~lexer() throw();
            
            const string name;
            int          line;
            
            lexical::scanner & declare(const string &id); //!< a new scanner
            lexical::scanner & declare(const char   *id); //!< a new scanner
            
            
            lexical::scanner & operator[](const string &id); //!< on the fly creation
            lexical::scanner & operator[](const char   *id); //!< on the fly creation
            const lexical::scanner & operator[](const string &id) const;
            const lexical::scanner & operator[](const char   *id) const;
            
            bool has(const string &id) const throw();    //!< check
            bool has(const char   *id) const;            //!< check
            
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
            
            //! is there a next lexeme
            const lexeme *peek(source &,ios::istream&);
            
            //! information
            const lexical::scanner &current() const throw();
            
            
        private:
            typedef set<string,lexical::scanner::pointer> scanDB;
            typedef list<lexical::scanner*>               history_type;
            lexical::scanner *scan;
            lexemes           cache;
            history_type      history;
            
        public:
            string last_label; //!< saved after a NEW lexeme
            string last_token; //!< saved after a NEW lexeme
            
        protected:
            scanDB            scanners;
            
        private:
            lexical::scanner *root;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            friend class plugin;
            
        public:
            p_dict                  dict;          //!< a shared dictionary
            const lexical::action   forward;       //!< emit token => lexeme
            const lexical::action   discard;       //!< drop token
            const lexical::action   newline;       //!< increase line, no lexeme
			const lexical::callback noop_cb;       //!< do nothing callback
			const lexical::callback endl_cb;       //!< increase line count on callback
            
            bool emit(const token&) throw();
            bool drop(const token&) throw();
            void on_newline(const token&) throw();      //!< ++line
			void do_nothing(const token &) throw();
            bool on_newline_drop(const token&) throw(); //!< newline, return false
            bool on_newline_emit(const token&) throw(); //!< newline, return true
        };
        
    }
}

#endif


