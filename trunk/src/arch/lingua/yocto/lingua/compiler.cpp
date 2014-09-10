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
            syntax::terminal & RULE_ID = terminal("ID", "[[:alpha:]][:word:]*");
            syntax::terminal & COLUMN  = jettison("COLUMN", ":");
            syntax::terminal & STOP    = jettison("STOP",";");
            syntax::terminal & EXPR    = terminal("EXPR", "[:cstring:]" );
            syntax::terminal & CHAR    = terminal("CHAR","'.'");
            syntax::terminal & LPAREN  = jettison("LPAREN", "\\(");
            syntax::terminal & RPAREN  = jettison("RPAREN", "\\)");
            syntax::terminal & PIPE    = jettison("PIPE", "\\|");
            syntax::terminal & CODE    = terminal("CODE","\\[[:word:]*\\]");
            
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
                CORE |= CHAR;
                CORE |= GROUP;
                
                syntax::aggregate &ATOM = agg("ATOM",syntax::is_merging_one);
                ATOM += CORE;
                ATOM += opt( terminal("ATTR", "[+?*]"));
                ATOM += opt( CODE );
                
                syntax::repeating &SEQUENCE = rep("SEQUENCE",ATOM,1);
                syntax::aggregate &ATOMS    = agg("ATOMS", syntax::is_merging_one);
                ATOMS += SEQUENCE;
                syntax::aggregate &ALT     = agg("ALT");
                ALT += PIPE;
                ALT += ATOMS;
                ATOMS += rep("EXTRA_CONTENT",ALT,0);
                
                GROUP += LPAREN;
                GROUP += ATOMS;
                GROUP += RPAREN;
                
                RULE += rep("BODY",ATOMS,1);
            }
            //------------------------------------------------------------------
            // end rule
            //------------------------------------------------------------------
            RULE += STOP;
            
            
            //------------------------------------------------------------------
            // create the grammar
            //------------------------------------------------------------------
            
            syntax::aggregate &GRAMMAR = agg("GRAMMAR");
            
            //------------------------------------------------------------------
            // with a parser declaration
            //------------------------------------------------------------------
            {
                syntax::aggregate &PARSER_DECL = agg("PARSER_DECL", syntax::is_merging_all);
                PARSER_DECL += terminal("PARSER", "@[:word:]+");
                PARSER_DECL += STOP;
                GRAMMAR += PARSER_DECL;
            }
            
            //------------------------------------------------------------------
            // make a dedicated node for RULES
            //------------------------------------------------------------------
            {
                syntax::aggregate &RULES = agg("RULES");
                RULES += rep("RULES_DECL", RULE, 1);
                GRAMMAR += RULES;
            }
            
            set_root( GRAMMAR );
            
            
        }
        
        void compiler:: do_nothing(const token &) throw() {}
        void compiler:: do_newline(const token &) throw() { ++line; }
        
        
        static inline
        syntax::xnode *__cleanup( syntax::xnode *node ) throw()
        {
            assert(node);
            if( ! node->terminal )
            {
                syntax::xnode::child_list &source = node->children();
                syntax::xnode::child_list  target;
                while(source.size)
                {
                    target.push_back( __cleanup( source.pop_front() ) );
                }
                target.swap_with(source);
            }
            else
            {
                if( node->label == "CHAR" || node->label == "EXPR")
                {
                    assert(node->lex()->size>=2);
                    node->lex()->skip();
                    node->lex()->trim();
                }
            }
            return node;
        }
        
        syntax::xnode * compiler::run( source &src )
        {
            
            syntax::xnode *node = parser::run(src);
            return __cleanup(simplify(assemble("ATOMS",assemble("ALT",rewrite(contract_atoms(node))))));
        }
        
        
        
        
    }
    
}

