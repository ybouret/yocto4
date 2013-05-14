#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lingua
    {
        
        counting:: ~counting() throw() {}
        
        counting:: counting( pattern *p, size_t a, size_t b) throw() :
        joker(tag,p),
        nmin(min_of(a,b)),
        nmax(max_of(a,b))
        {
            
        }
        
        counting:: counting( const counting &other ) :
        joker(other),
        nmin(other.nmin),
        nmax(other.nmax)
        {
        }
        
        counting * counting:: create( pattern *p, size_t a, size_t b)
        {
            auto_ptr<pattern> q(p);
            counting *ans = new counting(p,a,b);
            (void) q.yield();
            return ans;
        }
        
        void counting:: save(ios::ostream &fp) const
        {
            fp.emit(tag);
            fp.emit<uint32_t>(nmin);
            fp.emit<uint32_t>(nmax);
            write(fp);
        }
        
        pattern * counting:: clone() const
        {
            return new counting( *this );
        }
        
        bool counting:: accept( source &src )
        {
            assert(0==size);
            assert(0==motif->size);
            
            size_t n = 0;
            while( motif->accept(src) )
            {
                merge_back( *motif );
                assert(0==motif->size);
                ++n;
            }
            
            if( n >= nmin && n <=nmax )
            {
                return true;
            }
            else
            {
                src.unget( *this );
                return false;
            }
        }
        
        
        
        
    }
    
}
