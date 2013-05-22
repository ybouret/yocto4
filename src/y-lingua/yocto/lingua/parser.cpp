#include "yocto/lingua/parser.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        
        parser:: ~parser() throw()
        {
            
        }
        
        parser:: parser( const string &title, const string &main_id) :
        lexer(   title + " Lexer"),
        grammar( title + " Grammar"),
        scanner( declare(main_id) )
        {
            
        }
        
        syntax::terminal & parser:: terminal( const string &id, const string &expr, syntax::node_property ppty )
        {
            
            switch(ppty)
            {
                case syntax::is_merging_all:
                case syntax::is_merging_one:
                    throw exception("invalid syntax property for terminal '%s'", id.c_str());
                default:
                    break;
            }
            Y_LEX_FORWARD(scanner, id, expr);
            return term(id,ppty);
        }
        
        syntax::terminal & parser:: terminal( const char *id, const char *expr, syntax::node_property ppty )
        {
            const string ID(id);
            const string EX(expr);
            return  terminal(ID, EX, ppty);
        }
        
        syntax::terminal & parser:: univocal( const string &id, const string &expr )
        {
            return  terminal( id, expr, syntax::is_specialized);
        }
        
        syntax::terminal & parser:: univocal( const char *id, const char *expr )
        {
            const string ID(id);
            const string EX(expr);
            return univocal(ID, EX);
        }
        
        syntax::terminal & parser:: jettison( const string &id, const string &expr)
        {
            return terminal(id, expr, syntax::is_discardable);
        }
        
        syntax::terminal & parser:: jettison( const char *id, const char *expr )
        {
            const string ID(id);
            const string EX(expr);
            return jettison(ID, EX);
        }
        
        
        
        syntax::xnode *parser:: run( source &src )
        {
            int depth = 0;
            return  syntax::xnode::abstract( accept(*this,src),depth);
        }
        
        
    }
    
}

