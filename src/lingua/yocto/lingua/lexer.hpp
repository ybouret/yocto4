#ifndef YOCTO_LINGUA_LEXER_INCLUDED
#define YOCTO_LINGUA_LEXER_INCLUDED 1

#include "yocto/lingua/lexical/plugin.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/addr-list.hpp"

namespace yocto
{
    namespace lingua
    {

        //! transform a source of t_char in a source of lexemes
        class lexer : public object
        {
        public:
            explicit lexer(const string &id, const  string &root_id);
            explicit lexer(const char   *id, const  char   *root_id);
            virtual ~lexer() throw();
            const string name;
            int          line;

            void restart() throw();
            void stop()    throw();
            void call(const string &id); //!< change current scanner
            void back();                 //!< replace current scanner by last history

            lexical::scanner & declare(const string &);

            lexeme *get(source &src);
            void    unget(lexeme *lx) throw();
            void    unget(const lexical::scanner &subscan,
                          const int               created,
                          const string           &content);
            
            //const lexeme *peek();

            //__________________________________________________________________
            //
            //! registering plugins, all defined
            //__________________________________________________________________
            template <typename PLUGIN>
            inline lexical::plugin & load(const string &id)
            {
                return declare_plugin( new PLUGIN(id,*this) );
            }

            template <typename PLUGIN>
            inline lexical::plugin & load(const char *id)
            {
                const string Id(id);
                return load<PLUGIN>(Id);
            }

            //__________________________________________________________________
            //
            //! registering plugins with call expressions
            //__________________________________________________________________
            template <typename PLUGIN>
            inline lexical::plugin & load(const string &id,
                                          const string &expr)
            {
                return declare_plugin(new PLUGIN(id,*this,expr));
            }

            //! wrapper
            template <typename PLUGIN>
            inline lexical::plugin & load(const char *id,
                                          const char *expr)
            {
                const string Id(id);
                const string Expr(expr);
                return load<PLUGIN>(Id,Expr);
            }

            //__________________________________________________________________
            //
            //! registering plugins with call/back expressions
            //__________________________________________________________________
            template <typename PLUGIN>
            inline lexical::plugin & load(const string &id,
                                          const string &callExpr,
                                          const string &backExpr)
            {
                return declare_plugin(new PLUGIN(id,*this,callExpr,backExpr));
            }

            //! wrapper
            template <typename PLUGIN>
            inline lexical::plugin & load(const string &id,
                                          const char  *callExpr,
                                          const char  *backExpr)
            {
                const string CallExpr(callExpr);
                const string BackExpr(backExpr);
                return declare_plugin(new PLUGIN(id,*this,CallExpr,BackExpr));
            }




        private:
            typedef set<string,lexical::scanner::ptr> scanner_db;
            typedef addr_list<lexical::scanner>       history_type;

            lexical::scanner     *current;  //!< current scanner
            bool                  stopped;  //!< flag
            l_list                cache;    //!< lexemes
            history_type          history;  //!< history for sub-scanners
            lexical::scanner::ptr base;     //!< root scanner
            scanner_db            scdb;     //!< database
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            void setup();
            lexical::plugin & declare_plugin( lexical::plugin *plg );

        public:
            lexical::scanner &root;
            p_dict            dict;

        };
    }
}

#endif
