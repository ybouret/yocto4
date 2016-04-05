#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        static inline
        void simplify_logical(logical *p)
        {
            assert(p);
            p_list &ops = p->operands;
            p_list stk;
            while(ops.size)
            {
                stk.push_back( pattern::simplify(ops.pop_front()) );
            }
            ops.swap_with(stk);
        }

        pattern *pattern:: simplify(pattern *p) throw()
        {
            assert(p);
            assert(NULL!=p->self);
            switch(p->uuid)
            {
                case AND::UUID: {
                    AND *q = static_cast<AND *>(p->self);
                    simplify_logical(q);
                    if(1==q->operands.size)
                    {
                        p = q->operands.pop_front();
                        delete q;
                    }
                } break;

                case OR :: UUID:  {
                    OR *q = static_cast<OR *>(p->self);
                    simplify_logical(q);
                    if(1==q->operands.size)
                    {
                        p = q->operands.pop_front();
                        delete q;
                    }
                } break;

                case NONE:: UUID: {
                    simplify_logical( static_cast<NONE *>(p->self) );
                } break;

                default:
                    break;
            }
            return p;
        }
    }

}
