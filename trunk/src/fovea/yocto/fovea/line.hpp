#ifndef YOCTO_FOVEA_EDGE_INCLUDED
#define YOCTO_FOVEA_EDGE_INCLUDED 1

#include "yocto/fovea/vertex.hpp"

namespace yocto
{
    namespace fovea
    {
        
#if 0
        void check_edge(const VertexBase &a, const VertexBase &b);
        
        //! an edge from an existing mesh
        template <size_t DIM,typename T>
        class Edge
        {
        public:
            typedef Vertex<DIM,T>                    VTX;
            typedef typename vertex_for<DIM,T>::type vtx;
            
            const VTX &v1;
            const VTX &v2;
            
            inline Edge( const VTX &a, const VTX &b):
            v1( a ),
            v2( b )
            {
                check_edge(v1,v2);
            }
            
            inline Edge( const Edge &other ) throw() :
            v1(other.v1),
            v2(other.v2)
            {
            }
            
            
            inline ~Edge() throw() {}
            
            friend inline bool operator==( const Edge &lhs, const Edge &rhs ) throw()
            {
                return
                ( (&lhs.v1 == &rhs.v1) && ( &lhs.v2 == &rhs.v2) ) ||
                ( (&lhs.v1 == &rhs.v2) && ( &lhs.v2 == &rhs.v1) );
            }
            
            friend inline bool operator!=( const Edge &lhs, const Edge &rhs ) throw()
            {
                return ! (lhs==rhs);
            }
            
            inline bool joins( const Edge &other ) const throw()
            {
                assert(this!=&other);
                return
                (&v1 == & other.v1) ||
                (&v1 == & other.v2) ||
                (&v2 == & other.v1) ||
                (&v2 == & other.v2);
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(Edge);
        };
#endif
    }
}

#endif
