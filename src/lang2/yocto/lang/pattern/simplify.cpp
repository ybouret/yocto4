#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {


#if 0
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
#endif


        pattern *logical:: simplify( logical *p ) throw()
        {
            assert(p!=NULL);

            return p;
        }
        
    }
    
}
