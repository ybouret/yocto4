#include "yocto/lang/syntax/parse-node.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            
            void parse_node:: compress() throw()
            {
                if( terminal == 0 )
                {
                    child_list &chl = children();
                    child_list  tmp;
                    
                    //==========================================================
                    // Pass 1: remove discardable nodes
                    //==========================================================
                    while( chl.size > 0 )
                    {
                        parse_node *node = chl.pop_front();
                        if( node->flags == syntax::is_discardable )
                        {
                            delete node;
                        }
                        else 
                        {
                            // TODO: specialized
                            node->compress();
                            tmp.push_back(node);
                        }
                    }
                    
                    //==========================================================
                    // Pass 2: merge possible nodes
                    //==========================================================
                    while( tmp.size > 0 )
                    {
                        parse_node *node = tmp.pop_front();
                        if( node->flags == syntax::is_merging )
                        {
                            assert(node->terminal==0);
                            child_list &sub = node->children();
                            while( sub.size )
                            {
                                parse_node *child = sub.pop_front();
                                child->parent = this;
                                chl.push_back(child);
                            }
                            delete node;
                        }
                        else
                            chl.push_back(node);
                    }
                }
            }
            
        }
        
    }
    
}
