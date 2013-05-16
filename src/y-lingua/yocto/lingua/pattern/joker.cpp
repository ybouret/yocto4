#include "yocto/lingua/pattern/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        joker:: ~joker() throw()
        {
            assert(motif);
            delete motif;
            motif = 0;
        }
        
        joker:: joker( uint32_t t, pattern *p ) throw() :
        pattern(t),
        motif(p)
        {
            assert(motif);
            data = (void*)motif;
        }
        
        joker:: joker( const joker &jk ) :
        pattern(jk.type),
        motif( jk.motif->clone() )
        {
            data = (void*)motif;
        }
        
        void joker:: write( ios::ostream &fp ) const
        {
            motif->save(fp);
        }
        
        void joker:: reset() throw()
        {
            clear();
            motif->reset();
        }
        
        void joker:: optimize() throw()
        {
            motif->optimize();
        }
        
        
        void joker:: __viz( const void *parent, ios::ostream &fp ) const
        {
            motif->viz(fp);
            fp.viz(parent); fp << " -> "; fp.viz(motif); fp << ";\n";
        }
    }
}
