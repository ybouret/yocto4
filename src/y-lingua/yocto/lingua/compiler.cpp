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
            syntax::terminal & LPAREN  = jettison("LPAREN", "\\(");
            syntax::terminal & RPAREN  = jettison("RPAREN", "\\)");
            syntax::terminal & PIPE    = jettison("PIPE", "\\|");
            
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
                
                syntax::aggregate   & GROUP = agg( "GROUP", syntax::is_merging_one );
                syntax::alternative & CORE  = alt("CORE");
                CORE |= RULE_ID;
                CORE |= EXPR;
                CORE |= GROUP;
                
                syntax::aggregate &ATOM = agg("ATOM",syntax::is_merging_one);
                ATOM += CORE;
                ATOM += opt( terminal("ATTR", "[+?*]"));
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
                
            }
            //------------------------------------------------------------------
            // end rule
            //------------------------------------------------------------------
            RULE += STOP;
            
            
            
            set_root( rep("RULES",RULE,1) );
            
            
        }
        
        void compiler:: do_nothing(const token &) throw() {}
        void compiler:: do_newline(const token &) throw() { ++line; }
        
        namespace
        {
            ////////////////////////////////////////////////////////////////////
            // bottom up coupling
            ////////////////////////////////////////////////////////////////////
            static inline
            syntax::xnode * __merge_groups( syntax::xnode *node ) throw()
            {
                assert(node!=0);
                if( ! node->terminal )
                {
                    syntax::xnode::child_list  target;
                    syntax::xnode::child_list &source = node->children();
                    const bool is_group = ("GROUP" == node->label);
                    while( source.size )
                    {
                        syntax::xnode *sub = __merge_groups( source.pop_front() );
                        if( is_group && ("GROUP" == sub->label) )
                        {
                            syntax::xnode::child_list &ch = sub->children();
                            while( ch.size )
                            {
                                syntax::xnode *n  = ch.pop_front();
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
            
            ////////////////////////////////////////////////////////////////////
            // GROUP  consecutive coupling
            ////////////////////////////////////////////////////////////////////
            static inline
            syntax::xnode * __couple_groups( syntax::xnode *node ) throw()
            {
                assert(node);
                if( !node->terminal )
                {
                    syntax::xnode::child_list  target;
                    syntax::xnode::child_list &source = node->children();
                    while( source.size )
                    {
                        syntax::xnode *sub = __couple_groups( source.pop_front() );
                        syntax::xnode *prv = target.tail;
                        if( "GROUP" == sub->label && prv != 0 && "GROUP" == prv->label )
                        {
                            syntax::xnode::child_list &ch = sub->children();
                            while( ch.size )
                            {
                                syntax::xnode *n  = ch.pop_front();
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
            
            ////////////////////////////////////////////////////////////////////
            // ALT bottom-up coupling
            ////////////////////////////////////////////////////////////////////
            static inline
            syntax::xnode * __alt_merge( syntax::xnode *node ) throw()
            {
                if( !node->terminal )
                {
                    syntax::xnode::child_list  target;
                    syntax::xnode::child_list &source = node->children();
                    const bool is_alt = ( "ALT" == node->label );
                    while( source.size )
                    {
                        syntax::xnode *sub = __alt_merge( source.pop_front() );
                        if( is_alt && "ALT" == sub->label)
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
            
            
        }
        
        syntax::xnode * compiler:: ast( syntax::xnode *node )
        {
            assert(node);
            return __couple_groups(node);
        }
        
    }
    
}

