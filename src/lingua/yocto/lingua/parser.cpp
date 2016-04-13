#include "yocto/lingua/parser.hpp"


namespace yocto
{
    namespace lingua
    {
        parser:: ~parser() throw()
        {
        }

#define Y_PARSER_CTOR()
        
        parser:: parser(const string &lang_id, const string &root_id) :
        syntax::grammar(lang_id + ".grammar"),
        lexer(lang_id+".lexer",root_id)
        Y_PARSER_CTOR()
        {
        }

        parser:: parser(const char *lang_id, const char *root_id) :
        syntax::grammar(string(lang_id)+".grammar"),
        lexer( string(lang_id)+".lexer",root_id)
        Y_PARSER_CTOR()
        {
        }

        syntax::rule & parser::terminal(const string &label, const string &expr,const uint32_t flags)
        {
            //-- lexical
            root.emit(label, expr);

            //-- syntax
            return decl_term(label,flags);
        }

        syntax::rule & parser::terminal(const char *label, const char *expr,const uint32_t flags)
        {
            const string Label(label);
            const string Expr(expr);
            return terminal(Label,Expr,flags);
        }

        syntax::rule & parser::terminal(const string &expr,const uint32_t flags)
        {
            return terminal(expr,expr,flags);
        }

        syntax::rule & parser:: terminal(const char   *expr,const uint32_t flags)
        {
            const string Expr(expr);
            return terminal(Expr,flags);
        }

        syntax::alternate & parser::choice(const Rule &r1, const Rule &r2)
        {
            Alt &ans = alt();
            ans << r1 << r2;
            return ans;
        }

        syntax::alternate & parser::choice(const Rule &r1, const Rule &r2, const Rule &r3)
        {
            Alt &ans = alt();
            ans << r1 << r2 << r3;
            return ans;
        }


    }
}

namespace yocto
{
    namespace lingua
    {

        syntax::xnode *parser::parse( ios::istream &fp, const int start_line)
        {
            restart();
            line = start_line;
            source src(fp);
            return run(*this, src);
        }

    }

}
