#ifndef YOCTO_FOVEA_MESH_STRUCTURED_INCLUDED
#define YOCTO_FOVEA_MESH_STRUCTURED_INCLUDED


#include "yocto/fovea/cell.hpp"
#include "yocto/fovea/array3d.hpp"

namespace yocto
{
    namespace fovea
    {
       
        template <typename MESH>
        class structured
        {
        public:
            typedef typename MESH::layout_type    LAYOUT;
            static const size_t                   DIMS    = LAYOUT::DIMENSIONS;
            typedef typename MESH::TYPE           TYPE;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            typedef typename EDGE::DB             EDGE_DB;
            typedef Cell<DIMS,TYPE>               CELL;
            typedef typename CELL::List           CELL_LIST;
            
            static void assign( MESH &m )
            {
                __assign( m, int2type<DIMS>() );
            }
            
        private:
            static void __assign( MESH &m, int2type<1> )
            {
                const size_t num_edges = m.vertices - 1;
                EDGE_DB &edges = m.edges;
                edges.reserve(num_edges);
                
                for(unit_t i=m.lower,ip=m.lower+1;i<m.upper;++i,++ip)
                {
                    const size_t I0 = m.offset_of(i);
                    const size_t IP = m.offset_of(ip);
                    assert(I0<m.vertices);
                    assert(IP<m.vertices);
                    const VERTEX &v0 = m[I0];
                    const VERTEX &v1 = m[IP];
                    const EDGE edge( v0, v1 );
                    
                    if( !edges.insert(edge) )
                    {
                        m.throw_multiple_edges(I0,IP);
                    }
                    
                }
                assert( edges.size() == num_edges );

            }
            
        };
        
    }
}

#endif
