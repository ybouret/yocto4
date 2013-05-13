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
        
        joker:: joker( pattern *p ) throw() :
        motif(p)
        {
            assert(motif);
        }
        
        joker:: joker( const joker &jk ) :
        motif( jk.motif->clone() )
        {
        }
        
    }
}
