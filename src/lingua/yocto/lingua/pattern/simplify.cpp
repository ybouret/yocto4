#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        
        static inline
        pattern * __simplify( AND *p ) throw()
        {
            assert(p);
            p_list &ops = p->operands;
            p_list  stk;
            while(ops.size)
            {
                pattern *q = pattern::simplify(ops.pop_front());
                if(AND::UUID==q->uuid)
                {
                    AND *sub = static_cast<AND *>(q->self);
                    stk.merge_back(sub->operands);
                    delete q;
                }
                else
                {
                    stk.push_back(q);
                }
            }
            ops.swap_with(stk);
            if(1==ops.size)
            {
                pattern *q = ops.pop_front();
                delete p;
                return q;
            }
            else
            {
                return p;
            }
        }

        static inline
        pattern * __simplify( OR *p ) throw()
        {
            assert(p);
            p_list &ops = p->operands;
            p_list  stk;
            while(ops.size)
            {
                pattern *q = pattern::simplify(ops.pop_front());

                if(OR::UUID==q->uuid)
                {
                    OR *sub = static_cast<OR *>(q->self);
                    stk.merge_back(sub->operands);
                    delete q;
                }
                else
                {
                    stk.push_back(q);
                }
            }
            ops.swap_with(stk);
            if(1==ops.size)
            {
                pattern *q = ops.pop_front();
                delete p;
                return q;
            }
            else
            {
                return p;
            }

        }

        pattern * __simplify( NONE *p ) throw()
        {
            assert(p);
            p_list &ops = p->operands;
            p_list  stk;
            while(ops.size)
            {
                pattern *q = pattern::simplify(ops.pop_front());

                stk.push_back(q);
            }
            ops.swap_with(stk);
            return p;
        }






        pattern *pattern:: simplify(pattern *p) throw()
        {
            assert(p);
            assert(NULL!=p->self);
            switch(p->uuid)
            {
                case AND :: UUID: return __simplify( static_cast<AND  *>(p->self) );
                case OR  :: UUID: return __simplify( static_cast<OR   *>(p->self) );
                case NONE:: UUID: return __simplify( static_cast<NONE *>(p->self) );
                default: break;
            }
            return p;
        }
    }
    
}
