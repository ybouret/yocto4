#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    namespace lingua
    {
        optional:: ~optional() throw() {}
        
        optional:: optional( pattern *p ) throw() : joker(p) {}
        
        optional:: optional( const optional &other) :
        joker(other)
        {
        }
        
        pattern *optional:: clone() const { return new optional( *this ); }
        
        optional *optional:: create(pattern *p)
        {
            assert(p);
            auto_ptr<pattern> q(p);
            optional *ans = new optional(p);
            (void)q.yield();
            return ans;
        }
        
        bool optional:: accept( source &src )
        {
            assert(size==0);
            assert(motif->size==0);
           
            if(motif->accept(src))
            {
                swap_with(*motif);
                return true;
            }
            else
                return true;
        }
        
    }

}
