#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        pattern * pattern:: simplify(pattern *p) throw()
        {
            assert(p);
            p->optimize();
            switch(p->type)
            {
                case AND:: tag:
                case OR::  tag: {
                    assert(p->data);
                    p_list &operands = *static_cast<p_list *>( p->data );
                    if( operands.size == 1 )
                    {
                        pattern *q = operands.pop_back(); assert(0==operands.size);
                        delete p;
                        return q;
                    }
                } break;
                    
                default:
                    break;
            }
            return p;
        }
        
    }
    
}
