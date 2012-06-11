#include "yocto/lang/compiler.hpp"

namespace yocto 
{
    namespace  lang 
    {
        
        compiler:: ~compiler() throw()
        {
            
        }
        
        compiler:: compiler() :
        parser( "lang.compiler.lexer", "compiler.grammar" )
        {
            
            //------------------------------------------------------------------
            // scanner specifics
            //------------------------------------------------------------------
            scan.make( "BLANKS", "[ \t]+",    & scan.discard );
            scan.make( "ENDL"  , "[:endl:]+", & scan.newline );
            
            //------------------------------------------------------------------
            // declare terminals
            //------------------------------------------------------------------
            syntax::terminal  &RULEID   = terminal( "RULEID", "[:word:]+" );
            syntax::terminal  &REGEXP   = terminal( "REGEXP", "[:cstring:]");
            syntax::terminal  &SINGLE   = terminal( "SINGLE", "'[\\x20-\\x7F]'");
            syntax::terminal  &COLUMN   = terminal( ":",         ":", syntax::is_discardable );
            syntax::terminal  &END      = terminal( ";",         ";", syntax::is_discardable );
            syntax::terminal  &LPAREN   = terminal( "LPAREN",    "\\(", syntax::is_discardable );
            syntax::terminal  &RPAREN   = terminal( "RPAREN",    "\\)", syntax::is_discardable );
            syntax::terminal  &OPT      = terminal( "?", "\\?", syntax::is_specialized );
            syntax::terminal  &REP0     = terminal( "*", "\\*", syntax::is_specialized );
            syntax::terminal  &REP1     = terminal( "+", "\\+", syntax::is_specialized );
            syntax::terminal  &BAR      = terminal( "BAR",      "\\|", syntax::is_discardable);
            
            syntax::alternate &MODIF  = alt("MODIF");
            MODIF << OPT << REP0 << REP1;
            syntax::optional  &MODIFIER = opt("MODIFIER",MODIF);
            
            syntax::aggregate &RULE     = agg( "RULE" );
            //------------------------------------------------------------------
            // Rule prolog
            //------------------------------------------------------------------
            RULE << RULEID << COLUMN;
            
            //------------------------------------------------------------------
            // Rule content
            //------------------------------------------------------------------
            syntax::alternate &ATOM     = alt("ATOM");
            syntax::aggregate &ITEM     = agg("ITEM", syntax::is_merging_one );
            syntax::repeating &ELEMENTS = rep("ELEMENTS", ITEM, 1);
            syntax::aggregate &GROUP     = agg("GROUP", syntax::is_merging_one);
            syntax::aggregate &ALT = agg("ALT");
            ALT << BAR << ELEMENTS;
            syntax::repeating &OTHER= rep("OTHER", ALT,0);
            GROUP << LPAREN << ELEMENTS << RPAREN;
            ATOM << RULEID << REGEXP << SINGLE << GROUP;
            ITEM << ATOM << MODIFIER << OTHER;
            
            RULE &= ELEMENTS;
            
            
            //------------------------------------------------------------------
            // Rule epilog
            //------------------------------------------------------------------
            RULE &= END;
            
            //------------------------------------------------------------------
            // and the grammar
            //------------------------------------------------------------------
            syntax::repeating &GRAMMAR = rep("RULES",RULE,1);
            set_root(GRAMMAR);
        }
        
        static inline bool __is_modifier( const string &label ) throw()
        {
            return (label == "+") || (label == "?") || (label == "*");
        }

        
        static inline
        void __rewrite( syntax::parse_node *node )
        {
            assert(node!=NULL);
            if( ! node->terminal )
            {
                syntax::parse_node::child_list &chl = node->children();
                for( syntax::parse_node *sub = chl.head; sub; sub=sub->next )
                {
                    __rewrite(sub);
                }
                
                if( node->label == "ALT" )
                {
                    while(true)
                    {
                        syntax::parse_node *prev = node->prev;
                        assert( prev != NULL );
                        assert( prev->parent == node->parent );
                        std::cerr << "rewrite [ALT]: taking " << node->prev->label << std::endl;
                        chl.push_front( node->prev->unlink() );
                        prev->parent = node;
                        if( __is_modifier(prev->label) )
                            continue;
                        break;
                    }
                    
                }
            }
        }

        static inline
        void __fusion( syntax::parse_node *node )
        {
            assert(node!=NULL);
            if( ! node->terminal )
            {
                syntax::parse_node::child_list &chl = node->children();
                for( syntax::parse_node *sub = chl.head; sub; sub=sub->next )
                {
                    __fusion(sub);
                }
                
                if( node->label == "ALT" )
                {
                    
                }
            }
        }
        
        syntax::parse_node * compiler:: operator()( ios::istream &fp )
        {
            regex::source       Source( fp );
            reset();
            syntax::parse_node *Tree = accept(*this, Source);
            Tree->AST();
            __rewrite(Tree);
            Tree->AST();
            __fusion(Tree);
            return Tree;
        }
        
              
               
        
        
    }
    
}