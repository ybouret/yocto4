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
            syntax::rule & term(const char *label, const char *expr);

            //! restart and run
            syntax::xnode *run( ios::istream &fp );


        private:
            source src;
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
    }
    
}

#endif

