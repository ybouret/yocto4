#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    namespace lingua
    {
        
        at_least:: ~at_least() throw() {}
        
        at_least:: at_least( pattern *p, size_t n) throw() :
        joker(p),
        count(n)
        {
            
        }
        
        at_least:: at_least( const at_least &other ) :
        joker(other),
        count(other.count)
        {
        }
        
        at_least * at_least:: create( pattern *p, size_t n)
        {
            auto_ptr<pattern> q(p);
            at_least *ans = new at_least(p,n);
            (void) q.yield();
            return ans;
        }
        
        void at_least:: save(ios::ostream &fp) const
        {
            fp.emit(tag);
            motif->save(fp);
        }
        
        

    }

}
