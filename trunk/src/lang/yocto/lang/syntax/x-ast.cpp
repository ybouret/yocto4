#include "yocto/lang/syntax/xnode.hpp"


namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            static inline xnode *ast_terminal(xnode *node) throw()
            {
                assert(node);
                assert(node->terminal);
                switch(node->modifier)
                {
                    case is_discardable:
                        delete node;
                        return 0;
                        
                    case is_specialized:
                        node->lxm()->clear();
                        return node;
                        
                    default:
                        break;
                }
                return node;
            }
            
            
            static inline xnode *ast_compound(xnode *node) throw()
            {
                assert(node);
                assert(!node->terminal);
                xnode::child_list tmp;
                while( node->count() )
                {
                    xnode *child = xnode::ast( node->pop() );
                    if(!child)
                        continue;
                    
                    if(child->terminal)
                    {
                        child->parent = node;
                        tmp.push_back(child);
                    }
                    else
                    {
                        switch(child->modifier)
                        {
                            case is_merging_all:
                                while(child->count())
                                {
                                    xnode *sub = child->pop();
                                    sub->parent = node;
                                    tmp.push_back(sub);
                                }
                                delete child;
                                break;
                                
                            default:
                                child->parent = node;
                                tmp.push_back(child);
                        }
                    }
                }
                node->children().swap_with(tmp);
                return node;
            }
            
            xnode * xnode::ast( xnode *node ) throw()
            {
                if(!node) return 0;
                
                if(node->terminal)
                {
                    return ast_terminal(node);
                }
                else
                {
                    return ast_compound(node);
                }
            }
            
        }
        
    }
    
}
