#include "yocto/lingua/parser.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/pattern/basic.hpp"

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
        
        
        //======================================================================
        //
        // Generic Implementation
        //
        //======================================================================
        static inline
        void __check_terminal_ppty(const string         &id,
                                   syntax::node_property ppty)
        {
            switch(ppty)
            {
                case syntax::is_merging_all:
                case syntax::is_merging_one:
                    throw exception("invalid syntax property for terminal '%s'", id.c_str());
                default:
                    break;
            }
            
        }
        
        syntax::terminal & parser:: terminal(const string         &id,
                                             const string         &expr,
                                             syntax::node_property ppty )
        {
            __check_terminal_ppty(id, ppty);
            Y_LEX_FORWARD(scanner, id, expr);
            return term(id,ppty);
        }
        
        syntax::terminal & parser:: terminal(const char           *id,
                                             const char           *expr,
                                             syntax::node_property ppty )
        {
            const string ID(id);
            const string EX(expr);
            return  terminal(ID, EX, ppty);
        }
        
        syntax::terminal & parser:: terminal(const string         &id,
                                             const char            C,
                                             syntax::node_property ppty)
        {
            __check_terminal_ppty(id, ppty);
            const lexical::action fwd( &scanner, & lexical::scanner::__forward );
            scanner.make(id, single::create(C), fwd);
            return term(id,ppty);
        }
        
        syntax::terminal & parser:: terminal(const char           *id,
                                             const char            C,
                                             syntax::node_property ppty)
        {
            const string ID(id);
            return terminal(ID, C, ppty);
        }
        
        //======================================================================
        //
        // Univocal Implementation
        //
        //======================================================================
        syntax::terminal & parser:: univocal(const string &id,
                                             const string &expr)
        {
            return  terminal( id, expr, syntax::is_specialized);
        }
        
        syntax::terminal & parser:: univocal(const char *id,
                                             const char *expr )
        {
            const string ID(id);
            const string EX(expr);
            return univocal(ID, EX);
        }
        
        syntax::terminal & parser:: univocal(const string &id,
                                             const char    C )
        {
            return  terminal( id, C, syntax::is_specialized);
        }
        
        syntax::terminal & parser:: univocal(const char *id,
                                             const char  C )
        {
            return  terminal( id, C, syntax::is_specialized);
        }
        
        //======================================================================
        //
        // Jettison Implementation
        //
        //======================================================================
        syntax::terminal & parser:: jettison(const string &id,
                                             const string &expr)
        {
            return terminal(id, expr, syntax::is_discardable);
        }
        
        syntax::terminal & parser:: jettison(const char *id,
                                             const char *expr )
        {
            const string ID(id);
            const string EX(expr);
            return jettison(ID, EX);
        }
        
        
        syntax::terminal & parser:: jettison(const string &id,
                                             const char    C )
        {
            return terminal(id,C,syntax::is_discardable);
        }
        
        syntax::terminal & parser:: jettison(const char *id,
                                             const char  C )
        {
            return terminal(id,C,syntax::is_discardable);
        }
        
        
        syntax::xnode *parser:: run( source &src )
        {
            int depth = 0;
            return  syntax::xnode::AST( accept(*this,src), depth );
        }
        
        
    }
    
}

