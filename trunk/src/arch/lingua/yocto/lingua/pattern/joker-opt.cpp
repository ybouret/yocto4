#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        optional:: ~optional() throw() {}
        
        optional:: optional( pattern *p ) throw() : joker(tag,p) {}
        
        optional:: optional( const optional &other) :
        joker(other)
        {
        }
        
        void optional:: save(ios::ostream &fp) const
        {
            fp.emit(tag);
            write(fp);
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
        
        void optional:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [label=\"?\"];\n";
            __viz(this, fp);
        }
        
        void optional:: firsts( first_chars &fch ) const
        {
            fch.free();
            motif->firsts(fch);
            fch.accept_empty = true;
        }
        
        
    }

}
