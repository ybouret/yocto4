#include "yocto/lang//lexical/rule.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {

            rule:: rule( const string &l, pattern *p, const action &a ) :
            next(0),
            prev(0),

            label(l),
            motif(p),
            apply(a)
            {
                assert(motif!=0);
            }

            rule:: ~rule() throw()
            {
                assert(motif);
                delete motif;
                motif = 0;
            }


            rule * rule::create(const string &l, pattern *p, const action &a)
            {
                assert(p);
                try
                {
                    return new rule(l,p,a);
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

