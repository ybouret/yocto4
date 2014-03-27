#ifndef YOCTO_FOVEA_EDGE_INCLUDED
#define YOCTO_FOVEA_EDGE_INCLUDED 1

#include "yocto/fovea/vertex.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        
        template <size_t DIM,typename T>
        class edge
        {
        public:
            typedef Vertex<DIM,T>                    VTX;
            typedef typename vertex_for<DIM,T>::type vtx;
            
            const VTX &v1;
            const VTX &v2;
            const vtx mid;
            
            inline edge( const VTX &a, const VTX &b):
            v1( a.index < b.index ? a : b ),
            v2( a.index < b.index ? b : a ),
            mid()
            {
                if(v1.index==v2.index)
                    throw exception("same indices for edge");
                assert(v1.index<v2.index);
                /*
                 const vtx r1 = v1.make_pos();
                 const vtx r2 = v2.make_pos();
                 (vtx&)mid    = T(0.5) * (r1+r2);
                 */
            }
            
            inline edge( const edge &other ) throw() :
            v1(other.v1),
            v2(other.v2),
            mid(other.mid)
            {
            }
            
            
            inline ~edge() throw() {}
            
            friend inline bool operator==( const edge &lhs, const edge &rhs ) throw()
            {
                return (&lhs.v1 == &rhs.v1) && ( &lhs.v2 == &rhs.v2);
            }
            
            friend inline bool operator!=( const edge &lhs, const edge &rhs ) throw()
            {
                return (&lhs.v1 != &rhs.v1) || ( &lhs.v2 != &rhs.v2);
            }
            
            inline bool joins( const edge &other ) const throw()
            {
                assert(this!=&other);
                return
                (&v1 == & other.v1) ||
                (&v1 == & other.v2) ||
                (&v2 == & other.v1) ||
                (&v2 == & other.v2);
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(edge);
        };
        
    }
}

#endif
