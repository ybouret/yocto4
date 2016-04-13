#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/pattern/logic.hpp"

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

        syntax::rule & parser::maketerm(const string &label, const string &expr,const uint32_t flags)
        {
            //-- lexical
            root.emit(label, expr);

            //-- syntax
            return decl_term(label,flags);
        }


        syntax::rule & parser::terminal(const string &label, const string &expr)
        {
            return maketerm(label,expr,syntax::property::standard);
        }

        syntax::rule & parser::terminal(const char *label, const char *expr)
        {
            const string Label(label);
            const string Expr(expr);
            return maketerm(Label,Expr,syntax::property::standard);
        }

        syntax::rule & parser::univocal(const string &label, const string &expr)
        {
            return maketerm(label,expr,syntax::property::univocal);
        }

        syntax::rule & parser::univocal(const char *label, const char *expr)
        {
            const string Label(label);
            const string Expr(expr);
            return maketerm(Label,Expr,syntax::property::univocal);
        }


        syntax::rule & parser::univocal(const string &expr)
        {
            const lexical::action a( &root, & lexical::scanner::forward );
            pattern *p = logical::equal(expr);
            root.make(expr, p, a);

            return decl_term(expr,syntax::property::univocal);
        }

        syntax::rule & parser::univocal(const char *expr)
        {
            const string Expr(expr);
            return univocal(Expr);
        }

        syntax::rule & parser:: univocal(const char    C)
        {
            const string expr(&C,1);
            return univocal(expr);
        }

        syntax::rule & parser::jettison(const string &label, const string &expr)
        {
            return maketerm(label,expr,syntax::property::jettison);
        }

        syntax::rule & parser::jettison(const char *label, const char *expr)
        {
            const string Label(label);
            const string Expr(expr);
            return maketerm(Label,Expr,syntax::property::jettison);
        }


        syntax::rule & parser::jettison(const string &expr)
        {
            const lexical::action a( &root, & lexical::scanner::forward );
            pattern *p = logical::equal(expr);
            root.make(expr, p, a);

            return decl_term(expr,syntax::property::jettison);
        }

        syntax::rule & parser::jettison(const char *expr)
        {
            const string Expr(expr);
            return jettison(Expr);
        }

        syntax::rule & parser:: jettison(const char    C)
        {
            const string expr(&C,1);
            return jettison(expr);
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
