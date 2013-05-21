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

        void parser:: reset() throw()
        {
            lexer::reset();
            tree.release();;
        }
    
        bool parser:: operator()( source &src )
        {
            tree.release();
            syntax::xnode * root = 0;
            const bool      ans  = accept(*this, src, root);
            int depth = 0;
            std::cerr << "{" << grammar::name << "}: Abstracting" << std::endl;
            tree.reset( syntax::xnode::abstract(root,depth) );
            return ans;
        }
        
    }

}

