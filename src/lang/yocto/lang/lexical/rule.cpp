#include "yocto/lang//lexical/rule.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {

            rule:: rule( const string &l, pattern *p, const action &a , const bool is_ctrl) :
            next(0),
            prev(0),
            label(l),
            motif(p),
            apply(a),
            ctrl(is_ctrl)
            {
                assert(motif!=0);
            }

            rule:: ~rule() throw()
            {
                assert(motif);
                delete motif;
                motif = 0;
            }


            rule * rule::create(const string &l, pattern *p, const action &a, const bool is_ctrl)
            {
                assert(p);
                try
                {
                    return new rule(l,p,a,is_ctrl);
                }
                catch(...)
                {
                    delete p;
                    throw;
                }
            }

            
        }
    }
}

