#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {


        static pattern *simplify_fusion( logical *p ) throw()
        {
            assert(p);
            assert(p->uuid==AND::UUID || p->uuid==OR::UUID);
            assert(p->operands.size>=2);
            const uint32_t uuid = p->uuid;

            p_list ops;
            ops.swap_with(p->operands);

            while(ops.size)
            {
                pattern *q = ops.pop_front();
                if(uuid==q->uuid)
                {
                    assert(q->content());
                    p_list &children = *(p_list *)(q->content());
                    p->operands.merge_back(children);
                    delete q;
                }
                else
                {
                    p->operands.push_back(q);
                }
            }


            return p;
        }


        pattern *logical:: simplify( logical *p ) throw()
        {
            assert(p!=NULL);

            switch(p->uuid)
            {
                case AND::UUID:
                case OR ::UUID:
                    goto CHECK_FUSION;

                default:
                    break;
            }
            return p;
            
        CHECK_FUSION:
            switch(p->operands.size)
            {
                case 0:
                    return p;
                case 1: {
                    pattern *q = p->operands.pop_back();
                    delete p;
                    return q;
                }

                default:
                    break;
            }
            return simplify_fusion(p);
            
        }
        
    }
    
}
