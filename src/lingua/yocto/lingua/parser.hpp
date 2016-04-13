#ifndef YOCTO_LINGUA_PARSER_INCLUDED
#define YOCTO_LINGUA_PARSER_INCLUDED 1

#include "yocto/lingua/syntax/grammar.hpp"


namespace yocto
{
    namespace lingua
    {
        class parser : public syntax::grammar, public lexer
        {
        public:
            typedef syntax::rule      Rule;
            typedef syntax::aggregate Agg;
            typedef syntax::alternate Alt;

            virtual ~parser() throw();

            explicit parser(const string &lang_id, const string &root_id);
            explicit parser(const char   *lang_id, const char   *root_id);

            Rule &maketerm(const string &label, const string &expr,const uint32_t flags=syntax::property::standard);
            
            Rule &terminal(const string &label, const string &expr);
            Rule &terminal(const char   *label, const char   *expr);

            //! expression  is compiled
            Rule &univocal(const string &label, const string &expr);
            Rule &univocal(const char   *label, const char   *expr);

            //! expression is not compiled
            Rule &univocal(const string &expr);
            Rule &univocal(const char   *expr);
            Rule &univocal(const char    C);


            //! expression is compiled
            Rule &jettison(const string &label, const string &expr);
            Rule &jettison(const char   *label, const char   *expr);

            //! expression is not compiled
            Rule &jettison(const string &expr);
            Rule &jettison(const char   *expr);
            Rule &jettison(const char    C);

            
            Alt &choice(const Rule &, const Rule &);
            Alt &choice(const Rule &, const Rule &, const Rule &);

            syntax::xnode *parse( ios::istream &fp, const int start_line=1);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
    }
}

#endif
