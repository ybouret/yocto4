#include "yocto/lingua/lexical/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            rule:: ~rule() throw()
            {
                assert(0!=motif);
                delete motif;
                motif = 0;
            }


            rule:: rule(const string &id,
                        pattern      *p,
                        const action &a,
                        const bool    flag) :
            next(0),
            prev(0),
            label(id),
            motif(p),
            apply(a),
            ctrl(flag)
            {
                assert(0!=motif);
            }


            rule * rule::create(const string &id,
                                pattern      *p,
                                const action &a,
                                const bool    is_control)
            {
                assert(0!=p);
                try
                {
                    return new rule(id,p,a,is_control);
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
