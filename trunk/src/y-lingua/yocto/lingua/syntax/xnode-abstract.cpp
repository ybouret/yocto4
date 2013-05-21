#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/code/cast.hpp"
#include <cstring>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            xnode * xnode:: abstract(xnode *node)
            {
                if(!node) return 0;
                
                if(node->terminal)
                {
                    //==========================================================
                    // clean up terminal node
                    //==========================================================
                    switch(node->property)
                    {
                        case is_specialized:
                            node->lex()->clear();
                            break;
                            
                        case is_discardable:
                            delete node;
                            return 0;
                            
                        case is_regular:
                        default:
                            break;
                    }
                    return node;
                }
                else
                {
                    //==========================================================
                    // clean up non terminal node
                    //==========================================================
                    
                    
                    //----------------------------------------------------------
                    // recursive cleanup
                    //----------------------------------------------------------
                    xnode::child_list &source = node->children();
                    xnode::child_list  target;
                    while( source.size )
                    {
                        xnode *sub = xnode::abstract( source.pop_front() );
                        if(sub)
                        {
                            switch( sub->property )
                            {
                                case is_merging_one:
                                    assert(!sub->terminal);
                                    if( 1 == sub->children().size )
                                    {
                                        target.merge_back(sub->children());
                                        delete sub;
                                    }
                                    else
                                    {
                                        target.push_back(sub);
                                    }
                                    break;
                                    
                                case is_merging_all:
                                    assert(!sub->terminal);
                                    target.merge_back( sub->children() );
                                    delete sub;
                                    break;
                                    
                                default:
                                    target.push_back(sub);
                                    break;
                                    
                            }
                        }
                    }
                    source.swap_with(target);
                    
                    //----------------------------------------------------------
                    // do we have some content ?
                    //----------------------------------------------------------
                    if(0==source.size)
                    {
                        delete node;
                        return 0;
                    }
                    
                    //----------------------------------------------------------
                    //
                    //----------------------------------------------------------
                    
                    
                    return node;
                }
            }
        }
        
    }
    
}
