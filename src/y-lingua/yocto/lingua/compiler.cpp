#include "yocto/lingua/compiler.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        compiler:: ~compiler() throw() {}
        
        compiler:: compiler() :
        parser("compiler","main")
        {
            static const char CppComment[] = "C++ Comment";
            static const char C_Comment[]  = "C Comment";
            
            //==================================================================
            // lexer
            //==================================================================
            syntax::terminal & RULE_ID = terminal("RULE_ID", "[[:alpha:]][:word:]*");
            syntax::terminal & COLUMN  = jettison("COLUMN", ":");
            syntax::terminal & STOP    = jettison("STOP",";");
            syntax::terminal & EXPR    = terminal("EXPR", "[:cstring:]" );
            syntax::terminal & CHAR    = terminal("CHAR","'.'");
            syntax::terminal & LPAREN  = jettison("LPAREN", "\\(");
            syntax::terminal & RPAREN  = jettison("RPAREN", "\\)");
            syntax::terminal & PIPE    = jettison("PIPE", "\\|");
            syntax::terminal & CODE    = terminal("CODE","@[:word:]+");
            
            // Call C++ comment
            scanner.call(CppComment, "//",     this, &compiler::do_nothing);
            
            // Call C comment
            scanner.call(C_Comment, "/\\*\\*", this, &compiler::do_nothing);
            
            Y_LEX_DISCARD(scanner, "BLANK", "[:blank:]");
            Y_LEX_NO_ENDL(scanner);
            
            //==================================================================
            // C++ Comment
            //==================================================================
            lexical::scanner &COM1 = declare(CppComment);
            Y_LEX_DISCARD(COM1, "DOT", ".");
            COM1.back("[:endl:]", this, &compiler::do_newline);
            
            //==================================================================
            // C Comment
            //==================================================================
            lexical::scanner &COM2 = declare(C_Comment);
            Y_LEX_DISCARD(COM2, "DOT", ".");
            Y_LEX_NO_ENDL(COM2);
            COM2.back("\\*\\*/", this, &compiler::do_nothing);
            
            
            //==================================================================
            // grammar of grammar
            //==================================================================
            
            //------------------------------------------------------------------
            // start rule
            //------------------------------------------------------------------
            syntax::aggregate &RULE = agg("RULE");
            RULE += RULE_ID;
            RULE += COLUMN;
            
            
            //------------------------------------------------------------------
            // rule content
            //------------------------------------------------------------------
            {
#if 0
                syntax::aggregate   & GROUP = agg( "GROUP", syntax::is_merging_one );
                syntax::alternative & CORE  = alt("CORE");
                CORE |= RULE_ID;
                CORE |= EXPR;
                CORE |= CHAR;
                CORE |= GROUP;
                
                syntax::aggregate &ATOM = agg("ATOM",syntax::is_merging_one);
                ATOM += CORE;
                ATOM += opt( terminal("ATTR", "[+?*]"));
                ATOM += opt( CODE );
                
                syntax::repeating &ATOMS = rep("ATOMS",ATOM,1);
                syntax::aggregate &CONTENT = agg("CONTENT", syntax::is_merging_all);
                CONTENT += ATOMS;
                syntax::aggregate &ALT     = agg("ALT");
                ALT += PIPE;
                ALT += CONTENT;
                CONTENT += rep("EXTRA_CONTENT",ALT,0);
                
                GROUP += LPAREN;
                GROUP += CONTENT;
                GROUP += RPAREN;
                
                RULE += rep("BODY",CONTENT,1);
#endif
                
            }
            //------------------------------------------------------------------
            // end rule
            //------------------------------------------------------------------
            RULE += STOP;
            
            
            set_root( rep("RULES",RULE,1) );
            
            
        }
        
        void compiler:: do_nothing(const token &) throw() {}
        void compiler:: do_newline(const token &) throw() { ++line; }
        
        
        //======================================================================
        // contract consecutive groups on the same level
        //======================================================================
        static inline
        syntax::xnode * __contract_groups( syntax::xnode *node ) throw()
        {
            assert(node);
            if(!node->terminal)
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list  target;
                while(source.size)
                {
                    syntax::xnode *sub = __contract_groups( source.pop_front() );
                    syntax::xnode *prv = target.tail;
                    if( "GROUP" == sub->label && prv && "GROUP" == prv->label )
                    {
                        syntax::xnode::child_list &ch = sub->children();
                        while(ch.size)
                        {
                            syntax::xnode *n = ch.pop_front();
                            prv->children().push_back(n);
                            n->parent = prv;
                        }
                        delete sub;
                    }
                    else
                        target.push_back(sub);
                }
                source.swap_with(target);
            }
            return node;
        }
        
        //======================================================================
        // fusion group within a group
        //======================================================================
        static inline
        syntax::xnode * __assemble_groups( syntax::xnode *node ) throw()
        {
            assert(node);
            
            if( !node->terminal )
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list  target;
                const bool is_group = "GROUP" == node->label;
                while(source.size)
                {
                    syntax::xnode *sub = __assemble_groups( source.pop_front() );
                    if( is_group && "GROUP" == sub->label )
                    {
                        syntax::xnode::child_list &ch = sub->children();
                        while(ch.size)
                        {
                            syntax::xnode *n = ch.pop_front();
                            target.push_back(n);
                            n->parent = node;
                        }
                        delete sub;
                    }
                    else
                        target.push_back(sub);
                }
                target.swap_with(source);
            }
            
            return node;
        }
        
        //======================================================================
        // ALT rewriting
        //======================================================================
        static inline void __xnode_kill( syntax::xnode *node ) throw()
        {
            delete node;
        }
        
        
        static inline void __enhance( syntax::xnode *alt, const string &group_label )
        {
            assert(alt->label == "ALT" );
            syntax::xnode::child_list &source = alt->children();
            syntax::xnode::child_list  target;
            
            while(source.size&&source.head->label != "ALT")
            {
                target.push_back( source.pop_front() );
            }
            
            if( target.size > 1 && source.size > 0)
            {
                try
                {
                    syntax::xnode * g = syntax::xnode::create(group_label, 0, syntax::is_merging_one);
                    source.push_front(g);
                    g->parent = alt;
                    while(target.size)
                    {
                        g->children().push_back( target.pop_front() );
                        g->children().tail->parent = g;
                    }
                }
                catch(...)
                {
                    target.delete_with( __xnode_kill );
                    throw;
                }
            }
            else
            {
                // doesn't need to group
                while( target.size )
                    source.push_front( target.pop_back() );
            }
            
        }
        
        static inline
        syntax:: xnode * __rewrite( syntax::xnode *node, const string &group_label )
        {
            assert(node);
            if( !node->terminal )
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list  target;
                try {
                    
                    while(source.size)
                    {
                        target.push_back( __rewrite( source.pop_front(), group_label) );
                    }
                    
                    if( target.tail && "ALT" == target.tail->label )
                    {
                        __enhance(target.tail,group_label);
                    }
                    
                    target.swap_with(source);
                }
                catch(...)
                {
                    target.delete_with( __xnode_kill );
                    delete node;
                    throw;
                }
            }
            
            return node;
        }
        
        syntax::xnode * compiler::run( source &src )
        {
            //const string &group_label = (*this)["GROUP"].label;
            
            syntax::xnode *node = parser::run(src);
            return node;
        }
        
        
        
        
    }
    
}

