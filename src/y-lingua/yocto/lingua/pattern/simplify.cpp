#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        
        pattern * pattern:: simplify(pattern *p) throw()
        {
            assert(p);
            switch(p->type)
            {
                case AND:: tag:
                case OR::  tag: {
                    assert(p->data);
                    p_list &ops = *static_cast<p_list *>( p->data );
                    p_list  tmp;
                    while( ops.size )
                    {
                        tmp.push_back( pattern::simplify( ops.pop_front()) );
                    }
                    if( 1 == tmp.size )
                    {
                        delete p;
                        return tmp.pop_back();
                    }
                    ops.swap_with(tmp);
                } break;
                    
                default:
                    break;
            }
            return p;
        }
        
    }
    
}
