#include "yocto/lang/pattern.hpp"

#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {
        static inline
        void operands_optimize( p_list &ops ) throw()
        {
            p_list tmp;
            while(ops.size)
            {
                tmp.push_back( pattern::optimize(ops.pop_front() ) );
            }
            tmp.swap_with(ops);
        }
        
        static inline
        pattern *logic_optimize( logical *l ) throw()
        {
            p_list &ops = l->operands;
            operands_optimize(ops);
            if(ops.size==1)
            {
                pattern *p = ops.pop_back();
                delete   l;
                p->refactor();
                return   pattern::optimize(p);
            }
            else
            {
                return l;
            }
        }
        
        static inline
        pattern *joker_optimize( joker *jk ) throw()
        {
            jk->motif = pattern::optimize(jk->motif);
            return jk;
        }
        
        pattern * pattern:: optimize(pattern *p) throw()
        {
            assert(p);
            assert(p->self);
            switch(p->type)
            {
                    
                case optional::tag: return joker_optimize( static_cast<optional*>(p->self) );
                case at_least::tag: return joker_optimize( static_cast<at_least*>(p->self) );
                case counting::tag: return joker_optimize( static_cast<counting*>(p->self) );
                    
                case AND::tag: return logic_optimize(static_cast<AND*>(p->self) );
                case OR:: tag: return logic_optimize(static_cast<OR *>(p->self) );
                case NOT::tag: operands_optimize( static_cast<NOT *>(p->self)->operands );
                    break;
                    
                default:
                    break;
            }
            
            return p;
        }
    }
}