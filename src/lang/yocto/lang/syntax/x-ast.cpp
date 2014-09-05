#include "yocto/lang/syntax/xnode.hpp"


namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            
            xnode * xnode::ast( xnode *node ) throw()
            {
                if(!node) return 0;
                
                if(node->terminal)
                {
                    switch(node->modifier)
                    {
                        case is_regular:
                            return node;
                            
                        case is_discardable:
                            // shouldn't happen
                            delete node;
                            return 0;
                            
                        case is_specialized:
                            node->lxm()->clear();
                            return node;
                            
                        default:
                            // shouldn't happen
                        ;
                    }
                }
                else
                {
                    child_list &ch  = node->children();
                    {
                        child_list  tmp;
                        while(ch.size)
                        {
                            xnode *sub  = ch.pop_front();
                            sub->parent = 0;
                            xnode *res  = ast(sub);
                            if(res)
                            {
                                res->parent = node;
                                tmp.push_back(res);
                            }
                        }
                        ch.swap_with(tmp);
                    }
                    return node;
                }
            }
            
        }
        
    }
    
}
