#include "yocto/lang/compiler.hpp"

namespace yocto 
{
    namespace  lang 
    {
        
        compiler:: ~compiler() throw()
        {
            
        }
        
        void compiler:: enter_comment(const regex::token &) throw()
        {
        }
        
        void compiler:: leave_comment(const regex::token &) throw()
        {
            ++line;
        }
        
        
        static inline
        void __fusion( syntax::parse_node *node )
        {
            assert(node);
            if( ! node->terminal )
            {
                syntax::parse_node::child_list &chl = node->children();
                for( syntax::parse_node *curr = chl.head; curr; curr=curr->next )
                    __fusion(curr);
                
                if( node->label == "ALT" )
                {
                    syntax::parse_node::child_list tmp;
                    while( chl.size )
                    {
                        syntax::parse_node *curr = chl.pop_front();
                        if( curr->label == "ALT" )
                        {
                            syntax::parse_node::child_list &sub = curr->children();
                            while( sub.size )
                            {
                                syntax::parse_node *n = sub.pop_front();
                                n->parent = node;
                                tmp.push_back(n);
                            }
                            delete curr;
                        }
                        else
                        {
                            tmp.push_back(curr);
                        }
                    }
                    chl.swap_with(tmp);
                }
                
            }
        }
        
        
        compiler:: compiler() :
        parser( "lang.compiler.lexer", "lang.compiler.grammar" ),
        comment( declare("lang.compiler.comment") )
        {
            
            //------------------------------------------------------------------
            //
            // scanner specifics
            //
            //------------------------------------------------------------------
            scan.make( "BLANKS", "[ \t]+",    & scan.discard );
            scan.make( "ENDL"  , "[:endl:]+", & scan.no_endl );
            
            //------------------------------------------------------------------
            //
            // declare terminals
            //
            //------------------------------------------------------------------
            syntax::terminal  &RULEID   = terminal( "RULEID", "[:word:]+" );
            syntax::terminal  &REGEXP   = terminal( "REGEXP", "[:cstring:]");
            syntax::terminal  &SINGLE   = terminal( "SINGLE", "'[\\x20-\\x7F]'");
            syntax::terminal  &COLUMN   = terminal( ":",      ':', syntax::is_discardable );
            syntax::terminal  &END      = terminal( ";",      ';', syntax::is_discardable );
            syntax::terminal  &LPAREN   = terminal( "LPAREN", '(', syntax::is_discardable );
            syntax::terminal  &RPAREN   = terminal( "RPAREN", ')', syntax::is_discardable );
            syntax::terminal  &OPT      = terminal( "?",      '?', syntax::is_specialized );
            syntax::terminal  &REP0     = terminal( "*",      '*', syntax::is_specialized );
            syntax::terminal  &REP1     = terminal( "+",      '+', syntax::is_specialized );
            syntax::terminal  &BAR      = terminal( "BAR",    '|', syntax::is_discardable);
            
            //------------------------------------------------------------------
            //
            // hand coded comment
            //
            //------------------------------------------------------------------
            scan.call("lang.compiler.comment", "//", this, & compiler::enter_comment);
            
            comment.back("[:endl:]", this, & compiler::leave_comment );
            comment.make( "ANY1", ".", & comment.discard );
            
            //------------------------------------------------------------------
            //
            // declare modifiers: joker
            //
            //------------------------------------------------------------------
            syntax::alternate &MODIFIER  = alt("MODIFIER");
            MODIFIER << OPT << REP0 << REP1;
            syntax::optional  &OPT_MODIFIER = opt("OPT_MODIFIER",MODIFIER);
            
            //------------------------------------------------------------------
            //
            // declare optional code
            //
            //------------------------------------------------------------------
            syntax::terminal  &CODE      = terminal("CODE","@[[:word:][:digit:]]+");
            syntax::optional  &OPT_CODE  = opt("OPT_CODE",CODE);
            
            //------------------------------------------------------------------
            //
            // Start the rule
            //
            //------------------------------------------------------------------
            syntax::aggregate &RULE     = agg( "RULE" );
            
            //------------------------------------------------------------------
            // Rule prolog
            //------------------------------------------------------------------
            RULE << RULEID << COLUMN;
            
            
            //------------------------------------------------------------------
            // top level: alternation
            //------------------------------------------------------------------
            syntax::aggregate &ALT     = agg("ALT", syntax::is_merging_one);
            syntax::aggregate &SUB     = agg("SUB",syntax::is_merging_one);
            syntax::aggregate &OR      = agg("OR",  syntax::is_merging_one);
            OR  << BAR << SUB;
            ALT << SUB << rep("REP_OR",OR,0);
            
            
            
            //------------------------------------------------------------------
            // body description
            //------------------------------------------------------------------
            syntax::alternate &ATOM     = alt("ATOM");
            syntax::aggregate &ITEM     = agg("ITEM",  syntax::is_merging_one);
            syntax::aggregate &GROUP    = agg("GROUP", syntax::is_merging_one);
            GROUP << LPAREN << ALT << RPAREN;
            ATOM << RULEID << REGEXP << SINGLE << GROUP;
            ITEM << ATOM << OPT_MODIFIER << OPT_CODE;
            
            SUB  &= rep("ITEMS",ITEM,1);
            
            RULE &= ALT;
            
            //------------------------------------------------------------------
            // Rule epilog
            //------------------------------------------------------------------
            RULE &= END;
            
            //------------------------------------------------------------------
            // and the grammar is a set of rules...
            //------------------------------------------------------------------
            syntax::repeating &GRAMMAR = rep("RULES",RULE,1);
            set_root(GRAMMAR);
        }
        
        
        
        
        syntax::parse_node * compiler:: operator()( ios::istream &fp )
        {
            regex::source  Source( fp );
            reset();
            syntax::parse_node *Tree = accept(*this, Source);
            
            //-- first pass: take care of specialized and discardable
            Tree->AST();
            
            //-- merge alternation
            __fusion(Tree);
            
            return Tree;
        }
        
        
        
        
        
    }
    
}

