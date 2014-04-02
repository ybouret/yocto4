#ifndef YOCTO_FOVEA_MESH_STRUCTURED_INCLUDED
#define YOCTO_FOVEA_MESH_STRUCTURED_INCLUDED


#include "yocto/fovea/cell.hpp"
#include "yocto/fovea/array3d.hpp"
#include "yocto/fovea/box.hpp"

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
            //__________________________________________________________________
            //
            //
            // 1D
            //
            //__________________________________________________________________

            static void __assign( MESH &m, int2type<1> )
            {
                const size_t num_edges = m.vertices - 1;
                EDGE_DB &edges = m.edges;
                edges.reserve(num_edges);
                CELL_LIST &cells = m.cells;
                
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
                    
                    cells.push_back( new CELL(v0,v1) );
                    
                }
                assert( edges.size() == num_edges );
                
            }
            
            //__________________________________________________________________
            //
            //
            // 2D
            //
            //__________________________________________________________________
            
            static void __assign( MESH &m, int2type<2> )
            {
                EDGE_DB &edges = m.edges;
                const size_t nx        = m.width.x;
                const size_t ny        = m.width.y;
                const size_t x_edges   = nx - 1;
                const size_t y_edges   = ny - 1;
                const size_t c_edges   = x_edges * y_edges;
                const size_t num_edges = ny * x_edges + nx * y_edges + c_edges;
                m.edges.reserve(num_edges);
                //--------------------------------------------------------------
                // first pass: all edges
                //--------------------------------------------------------------

                //! X edges
                for(unit_t j=m.lower.y;j<=m.upper.y;++j)
                {
                    for(unit_t i=m.lower.x,ip=m.lower.x+1;i<m.upper.x;++i,++ip)
                    {
                        const coord2D C0(i,j);
                        const coord2D C1(ip,j);
                        const size_t  I0 = m.offset_of(C0);
                        const size_t  I1 = m.offset_of(C1);
                        assert(I0<m.vertices);
                        assert(I1<m.vertices);
                        const EDGE edge( m[I0], m[I1] );
                        if( !m.edges.insert(edge) )
                        {
                            m.throw_multiple_edges(I0,I1);
                        }
                    }
                }
                
                
                //! Y edges
                for(unit_t j=m.lower.y,jp=m.lower.y+1;j<m.upper.y;++j,++jp)
                {
                    for(unit_t i=m.lower.x;i<=m.upper.x;++i)
                    {
                        const coord2D C0(i,j);
                        const coord2D C1(i,jp);
                        const size_t  I0 = m.offset_of(C0);
                        const size_t  I1 = m.offset_of(C1);
                        assert(I0<m.vertices);
                        assert(I1<m.vertices);
                        const EDGE edge( m[I0], m[I1] );
                        if( !m.edges.insert(edge) )
                        {
                            m.throw_multiple_edges(I0,I1);
                        }
                    }
                }
                
                //! C edges
                for(unit_t j=m.lower.y,jp=m.lower.y+1;j<m.upper.y;++j,++jp)
                {
                    for(unit_t i=m.lower.x,ip=m.lower.x+1;i<m.upper.x;++i,++ip)
                    {
                        const coord2D C00(i,j);
                        const coord2D C11(ip,jp);
                        const size_t I00 = m.offset_of(C00);
                        const size_t I11 = m.offset_of(C11);
                        assert(I00<m.vertices);
                        assert(I11<m.vertices);
                        const EDGE edge( m[I00], m[I11] );
                        if( !m.edges.insert(edge) )
                        {
                            m.throw_multiple_edges(I00,I11);
                        }
                        
                    }
                }
                assert( edges.size() == num_edges );

                //--------------------------------------------------------------
                // second pass: all cells
                //--------------------------------------------------------------
                CELL_LIST &cells = m.cells;
                for(unit_t j=m.lower.y,jp=m.lower.y+1;j<m.upper.y;++j,++jp)
                {
                    for(unit_t i=m.lower.x,ip=m.lower.x+1;i<m.upper.x;++i,++ip)
                    {
                        const coord2D C00(i,j);
                        const coord2D C01(i,jp);
                        const coord2D C10(ip,j);
                        const coord2D C11(ip,jp);
                        const size_t I00 = m.offset_of(C00);
                        const size_t I01 = m.offset_of(C01);
                        const size_t I10 = m.offset_of(C10);
                        const size_t I11 = m.offset_of(C11);
                        assert(I00<m.vertices);
                        assert(I01<m.vertices);
                        assert(I10<m.vertices);
                        assert(I11<m.vertices);
                        const VERTEX &v00 = m[I00];
                        const VERTEX &v01 = m[I01];
                        const VERTEX &v10 = m[I10];
                        const VERTEX &v11 = m[I11];

                        // make two CCW triangles
                        cells.push_back( new CELL(v00,v01,v10) );
                        cells.push_back( new CELL(v10,v11,v01) );
                    }
                }
            }
            
            //__________________________________________________________________
            //
            //
            // 3D
            //
            //__________________________________________________________________
            
        };
        
    }
}

#endif
