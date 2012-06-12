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
        
        
        
        static inline
        void __rewrite( syntax::parse_node *node )
        {
            assert(node);
            if( ! node->terminal )
            {
                syntax::parse_node::child_list &chl = node->children();
                syntax::parse_node::child_list  tmp;
                while( chl.size )
                {
                    syntax::parse_node *curr = chl.pop_front();
                    if( curr->label == "ITEM" )
                    {
                        syntax::parse_node::child_list &sub = curr->children();
                        if( sub.tail->label == "ALT" )
                        {
                            //-- effective rewriting
                            syntax::parse_node *alt = sub.pop_back();
                            alt->parent = node;
                            tmp.push_back(alt);
                            alt->children().push_front(curr);
                            curr->parent = alt;
                            continue;
                        }
                    }
                    
                    //-- default
                    tmp.push_back( curr );
                }
                
                while( tmp.size > 0 )
                {
                    syntax::parse_node *curr = tmp.pop_front();
                    chl.push_back(curr);
                    __rewrite(curr);
                }
            }
        }
        
        static inline
        void __fusion( syntax::parse_node *node )
        {
            assert(node);
            if( ! node->terminal )
            {
                syntax::parse_node::child_list &chl = node->children();
                
                //-- depth first
                for( syntax::parse_node *curr = chl.head; curr; curr=curr->next)
                    __fusion(curr);
                
                //-- look for possible ALT fusion
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
                            tmp.push_back( curr );
                    }
                    chl.swap_with(tmp);
                }
                
            }
        }
        
        
        
        syntax::parse_node * compiler:: operator()( ios::istream &fp )
        {
            regex::source       Source( fp );
            reset();
            syntax::parse_node *Tree = accept(*this, Source);
            
            //-- first pass: take care of specialized and discardable
            Tree->AST();
            
            //-- rewrite alternate
            __rewrite(Tree);
            
            //-- second pass: fusion single new items
            Tree->AST();
            
            //-- third pass: fusion
            __fusion(Tree);
            
            return Tree;
        }
        
        
        
        
        
    }
    
}

