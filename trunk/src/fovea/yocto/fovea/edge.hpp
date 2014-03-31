#ifndef YOCTO_FOVEA_EDGE_INCLUDED
#define YOCTO_FOVEA_EDGE_INCLUDED 1

#include "yocto/fovea/vertex.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace fovea
    {
        
        //! POD key
        class edge_key
        {
        public:
            const size_t i1; //!< first index
            const size_t i2; //!< a1<a2
            edge_key(size_t a1, size_t a2);
            edge_key(const edge_key &) throw();
            ~edge_key() throw();
            friend bool operator==( const edge_key &lhs, const edge_key &rhs) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(edge_key);
        };
        
        
        
        template <size_t DIM,typename T>
        class Edge
        {
        public:
            typedef Vertex<DIM,T>        VERTEX;
            typedef Edge<DIM,T>          EDGE;
            typedef typename VERTEX::vtx vtx;
            
            const VERTEX   &v1;
            const VERTEX   &v2;
            const edge_key  ek;
            const T         length;
            const vtx       middle;
            
            inline Edge(const VERTEX &u1, const VERTEX &u2) :
            v1( u1.index < u2.index ? u1 : u2 ),
            v2( u1.index < u2.index ? u2 : u1 ),
            ek(v1.index,v2.index),
            length(0),
            middle()
            {
                assert(ek.i1==v1.index);
                assert(ek.i2==v2.index);
            }
            
            const edge_key &key() const throw() { return ek; }
            
            inline Edge( const Edge &other ) throw() :
            v1(other.v1),
            v2(other.v2),
            ek(other.ek),
            length(other.length),
            middle(other.middle)
            {
            }
            
            // once the vertices are loaded
            inline void load() const throw()
            {
                T         sum = 0;
                const vtx dif = v2.r - v1.r;
                const T  *d   = (T *)&dif;
                const T  *o   = (T *)&v1.r;
                T        *m   = (T *)&middle;
                for(size_t i=0;i<DIM;++i)
                {
                    const T dd = d[i];
                    sum += dd * dd;
                    m[i] = o[i] + dd/2;
                }
                (T &)length = math::Sqrt(sum);
            }
            
            typedef set<edge_key,EDGE,key_hasher<edge_key,hashing::sfh>,memory_allocator> DB;
            
            
        private:
            YOCTO_DISABLE_ASSIGN(Edge);
        };
        
    }
}

#endif
