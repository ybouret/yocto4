#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"


namespace yocto
{
    namespace lingua
    {
#if 0
        pattern *pattern:: optimize( pattern *p )
        {
            assert(p);
            //------------------------------------------------------------------
            // pass-1: recursive optimize
            //------------------------------------------------------------------
            switch(p->type)
            {
                case AND:: tag:
                case OR::  tag:
                case NOT:: tag: {
                    assert(p->data);
                    p_list &ops = *static_cast<p_list *>(p->data);
                    p_list tmp;
                    while( ops.size )
                    {
                        tmp.push_back( pattern::optimize(ops.pop_front()) );
                    }
                    ops.swap_with(tmp);
                } break;
                    
                case optional::tag:
                case at_least::tag:
                case counting::tag:
                    assert(p->data);
                    static_cast<pattern *>(p->data)->optimize;
                    break;
                    
                default:
                    break;
            }
            
            //------------------------------------------------------------------
            // pass-2: special cases
            //------------------------------------------------------------------
            switch(p->type)
            {
                case AND::tag:
                    
                    break;
                    
                default:
                    break;
            }
            
            return p;
        }
#endif
        
    }
}
