#ifndef YOCTO_LANG_PARSER_INCLUDED
#define YOCTO_LANG_PARSER_INCLUDED 1

#include "yocto/lang/syntax/grammar.hpp"


namespace yocto
{

    namespace lang
    {

        class parser : public syntax::grammar, public lexer
        {
        public:
            typedef syntax::rule      Rule; //!< for derived class
            typedef syntax::aggregate Agg;  //!< for derived class
            typedef syntax::alternate Alt;  //!< for derived class
            
            lexical::scanner &scanner;
            explicit parser(const string &langID, const string &mainScanner);
            explicit parser(const char   *langID, const char   *mainScanner);
            virtual ~parser() throw();


            //! a terminal
            syntax::terminal & term(const char *label, const char *expr, syntax::property ppty = syntax::standard );


            //! a terminal from a plugin
            template <typename PLUGIN>
            syntax::terminal  &term(const char *label, syntax::property ppty = syntax::standard)
            {
                lexical::plugin &plg = load<PLUGIN>(label);
                plg.hook(scanner);
                return decl_term(label,ppty);
            }

            //! shortcut
            syntax::alternate & choice(Rule &r1, Rule &r2);

            //! shortcut
            syntax::alternate & choice(Rule &r1, Rule &r2, Rule &r3);
            

            //! clear all and accept top level rule
            syntax::xnode *run( ios::istream &fp );

            static parser *generate( const char *langID, ios::istream &fp, const bool output_files=false);
            static parser *generate( const char *langID, const char *data, const size_t size,const bool output_files=false);

        private:
            source src;
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
    }
    
}

#endif

