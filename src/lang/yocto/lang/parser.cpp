#include "yocto/lang/parser.hpp"

namespace yocto
{
    namespace lang
    {

        parser:: ~parser() throw()
        {
        }



        parser:: parser(const string &langID, const string &mainScanner) :
        syntax::grammar(langID+".grammar"),
        lexer(          langID+".lexer", mainScanner ),
        scanner( get_root() ),
        src()
        {
        }

        parser:: parser(const char *langID, const char *mainScanner) :
        syntax::grammar(string(langID)+".grammar"),
        lexer(          string(langID)+".lexer", string(mainScanner) ),
        scanner( get_root() ),
        src()
        {
        }

    }
}

namespace yocto
{
    namespace lang
    {

        syntax::terminal & parser:: term(const char *label, const char *expr, syntax::property ppty)
        {
            // make a lexical rule
            scanner.emit(label, expr);

            // make a grammar rule
            return grammar::decl_term(label,ppty);
        }

        syntax::terminal & parser:: term_is(const char *expr,syntax::property ppty  )
        {
            return term(expr,expr,ppty);
        }


        syntax::alternate & parser:: choice(Rule &r1, Rule &r2)
        {
            syntax::alternate &r = alt();
            r << r1 << r2;
            return r;
        }

        syntax::alternate & parser:: choice(Rule &r1, Rule &r2, Rule &r3)
        {
            syntax::alternate &r = alt();
            r << r1 << r2 << r3;
            return r;
        }
        
        syntax::xnode * parser::run(ios::istream &fp)
        {
            restart();
            src.clear();
            lexer::clear();
            
            syntax::xnode *ast = accept(*this, src, fp);

            return ast;
        }

    }

}
