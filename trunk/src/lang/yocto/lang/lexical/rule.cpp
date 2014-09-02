#include "yocto/lang/lexical/rule.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            rule:: rule(const string &id, pattern *pp, const action &cb, const bool flag ) :
            label(id),
            motif(pp),
            deed(cb),
            ctrl(flag),
            next(0),
            prev(0)
            {
                
            }
            
            rule:: ~rule() throw()
            {
                assert(motif);
                assert(0==next);
                assert(0==prev);
                delete motif;
                motif = 0;
            }
            
            rule * rule:: create(const string &id, pattern *pp, const action &cb, const bool flag)
            {
                assert(pp);
                try
                {
                    return new rule(id,pp,cb,flag);
                }
                catch(...)
                {
                    delete pp;
                    throw;
                }
            }
            
            

        }
        
    }
}
