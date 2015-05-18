#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/addr-list.hpp"

namespace yocto
{
    namespace lang
    {

        //! lexer
        /**
         use of multiple scanners to convert
         a source of t_char into a source of tokens.
         */
        class lexer : public object, public l_list
        {
        public:
            explicit lexer(const string &id, const string &root_scanner);
            explicit lexer(const char   *id, const char   *root_scanner);
            virtual ~lexer() throw();

            const string name;
            int          line;

            void restart() throw();

            lexical::scanner & get_root() throw();

            //! create a new scanner
            lexical::scanner & declare(const string &id);

            //! create a new scanner, wrapper
            lexical::scanner & declare(const char   *id);


            //! jump to another scanner
            void jump(const string &id);

            //! call another scanner
            void call(const string &id);

            //! back from another scanner
            void back();

            //! from cache or new
            lexeme *get(source &src, ios::istream &fp);

            //! restore a lexeme
            void    unget( lexeme * ) throw();

            //! restore a copy of a lexeme
            void    uncpy( const lexeme *);

            //! mimic a lexeme unget
            void    unget(const lexical::plugin &, const string &content);

            //! is there anyone left ?
            const lexeme *peek(source &src, ios::istream &fp);

            
            template <typename PLUGIN> inline
            lexical::plugin & load(const char *id)
            {
                lexical::plugin *plg = new PLUGIN(id,*this);
                declare_plugin(plg);
                return *plg;
            }

            template <typename PLUGIN> inline
            lexical::plugin & load(const char *id, const char *expr)
            {
                lexical::plugin *plg = new PLUGIN(id,*this,expr);
                declare_plugin(plg);
                return *plg;
            }

            template <typename PLUGIN> inline
            lexical::plugin & load(const char *id, const char *ini_expr, const char *end_expr)
            {
                lexical::plugin *plg = new PLUGIN(id,*this,ini_expr,end_expr);
                declare_plugin(plg);
                return *plg;
            }

            void dump() const throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            typedef lexical::scanner::ptr       p_scanner;
            typedef set<string,p_scanner>       scannerDB;
            typedef addr_list<lexical::scanner> historyDB;

            lexical::scanner *scan;     //!< active scanner
            lexical::scanner *Root;     //!< root scanner
            scannerDB         scanners;
            historyDB         history;

            void initialize(const string &root_scanner);
            void declare_plugin( lexical::plugin *plg);

        public:
            p_dict dict;
        };

    }

}

#endif

