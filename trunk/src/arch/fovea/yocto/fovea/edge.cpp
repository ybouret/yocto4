#include "yocto/fovea/edge.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        edge_key:: edge_key( size_t a1, size_t a2 ) :
        i1(a1), i2(a2)
        {
            if(i1==i2)
                throw exception("Invalid Edge Key: Twice #%u", unsigned(i1) );
            if(i1>i2)
                cswap_const(i1, i2);
        }
        
        
        edge_key:: edge_key( const edge_key &ek ) throw() :
        i1(ek.i1),
        i2(ek.i2)
        {
        }
        
        edge_key:: ~edge_key() throw() {}
        
        bool operator==( const edge_key &lhs, const edge_key &rhs) throw()
        {
            return (lhs.i1 == rhs.i1) && (lhs.i2==rhs.i2);
        }
    }
}
