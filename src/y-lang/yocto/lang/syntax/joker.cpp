#include "yocto/lang/syntax/joker.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax
        {
            
            joker:: joker( const string &id, rule &r ) :
            rule(id),
            ref(r)
            {
            }
            
            joker:: ~joker() throw()
            {
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // optional (?)
            //
            ////////////////////////////////////////////////////////////////////
            optional:: optional( const string &id, rule &r ) :
            joker( id, r )
            {
            }
            
            optional:: ~optional() throw()
            {
            }
            
            bool optional:: match( Y_SYNTAX_MATCH_ARGS )
            {
                //std::cerr << "?OPT <" << ref.label << "> @" << label << "" << std::endl;
                
                parse_node *Node = NULL;
                if( ref.match(Lexer, Source, Node) )
                {
                    grow(Tree,Node);
                }
                // always successfull
                return true;
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // repeating
            //
            ////////////////////////////////////////////////////////////////////
            repeating:: repeating( const string &id, rule &r, size_t at_least ) :
            joker(id,r),
            min_count(at_least)
            {
                
            }
            
            repeating:: ~repeating() throw() {}
            
            bool repeating:: match( Y_SYNTAX_MATCH_ARGS )
            {
                //std::cerr << "?REP>=" << min_count << " <" << ref.label << ">  @" << label << "" << std::endl;
                check(Tree);
                
                
                size_t count = 0;
                // create a local tree
                parse_node *sub_tree = new parse_node(label,NULL,is_regular);
                try
                {
                    // accept as many as possible
                    while( ref.match(Lexer, Source, sub_tree) )
                        ++count;
                }
                catch(...)
                {
                    delete sub_tree;
                    throw;
                }
                
                // check requirements
                if( count < min_count )
                {
                    // forget it...
                    parse_node::restore(Lexer, sub_tree);
                    return false;
                }
                else 
                {
                    if( NULL == Tree )
                    {
                        // replacement
                        Tree = sub_tree;
                    }
                    else
                    {
                        // direct fusion
                        assert( ! Tree->terminal );
                        parse_node::child_list &src = sub_tree->children();
                        parse_node::child_list &tgt = Tree->children();
                        while( src.size )
                        {
                            parse_node *node = src.pop_front();
                            node->parent     = Tree;
                            tgt.push_back( node );
                        }
                        delete sub_tree;
                    }
                    return true;
                }
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // zero_or_more
            //
            ////////////////////////////////////////////////////////////////////
            zero_or_more:: zero_or_more( const string &id, rule &r ) :
            repeating( id, r, 0 )
            {
            }
            
            zero_or_more:: ~zero_or_more() throw()
            {
            }
            
            
            ////////////////////////////////////////////////////////////////////
            //
            // one or more
            //
            ////////////////////////////////////////////////////////////////////
            one_or_more:: one_or_more( const string &id, rule &r ) :
            repeating( id, r, 1 )
            {
            }
            
            one_or_more:: ~one_or_more() throw()
            {
            }

            
            
            
        }
    }
    
}
