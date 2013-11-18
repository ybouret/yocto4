#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        
        at_least:: ~at_least() throw() {}
        
        at_least:: at_least( pattern *p, size_t n) throw() :
        joker(tag,p),
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
            fp.emit<uint32_t>(count);
            write(fp);
        }
        
        pattern * at_least:: clone() const
        {
            return new at_least( *this );
        }
        
        bool at_least:: accept( source &src )
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
            
            if( n >= count )
            {
                return true;
            }
            else
            {
                src.unget( *this );
                assert(0==size);
                assert(0==motif->size);
                return false;
            }
        }
        
        pattern * zero_or_more( pattern *p )
        {
            return at_least::create(p, 0);
        }
        
        pattern * one_or_more( pattern *p )
        {
            return at_least::create(p, 1);
        }
        
        void at_least:: viz( ios::ostream &fp) const
        {
            fp.viz(this);
            fp(" [label=\">=%u\"];\n", unsigned(count));
            __viz(this, fp);
        }

        void at_least:: firsts( first_chars &fch ) const
        {
            fch.free();
            motif->firsts(fch);
            fch.accept_empty = count <= 0;
        }
        
        
    }
    
}
