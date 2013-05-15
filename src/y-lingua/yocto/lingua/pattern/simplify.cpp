#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        
        pattern * pattern:: collapse(pattern *p) throw()
        {
            assert(p);
            switch(p->type)
            {
                case AND:: tag:
                case OR::  tag:
                {
                    assert(p->data);
                    p_list &ops = *static_cast<p_list *>( p->data );
                    if(ops.size==1)
                    {
                        pattern *q = ops.pop_back();
                        delete p;
                        p=q;
                    }
                }
                    break;
                    
                default:
                    break;
            }
            return p;
        }
        
    }
    
}
