#ifndef YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED 1

#include "yocto/fovea/mesh/structured.hpp"


namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class curvilinear_mesh : public Mesh<LAYOUT::DIMENSIONS,T>, public LAYOUT
        {
        public:
            typedef typename array_for<LAYOUT::DIMENSIONS,T>::type   array_type;
            typedef LAYOUT                        layout_type;
            typedef curvilinear_mesh<T,LAYOUT>    mesh_type;
            typedef Mesh<LAYOUT::DIMENSIONS,T>    MESH;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            typedef Cell<LAYOUT::DIMENSIONS,T>    CELL;
            typedef typename CELL::List           CELL_LIST;
            
            // public data
            const LAYOUT & get_layout() const throw() { return *this; }
            CELL_LIST      cells;
            
            
            inline explicit curvilinear_mesh( array_db &a, const LAYOUT &L ) :
            MESH(a,
                 L.items,
                 mesh::is_curvilinear),
            LAYOUT(L),
            cells()
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string array_n = this->axis_name(i);
                    this->adb.store( new array_type(array_n, *this) );
                }
                assign( int2type<LAYOUT::DIMENSIONS>() );
                this->assigned = true;
            }
            
            inline virtual ~curvilinear_mesh() throw()
            {
            }
            
            inline array_type       &X()       { return this->adb[ mesh::axis_name(0) ].template as<array_type>(); }
            inline const array_type &X() const { return this->adb[ mesh::axis_name(0) ].template as<array_type>(); }
            inline array_type       &Y()       { return this->adb[ mesh::axis_name(1) ].template as<array_type>(); }
            inline const array_type &Y() const { return this->adb[ mesh::axis_name(1) ].template as<array_type>(); }
            inline array_type       &Z()       { return this->adb[ mesh::axis_name(2) ].template as<array_type>(); }
            inline const array_type &Z() const { return this->adb[ mesh::axis_name(2) ].template as<array_type>(); }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(curvilinear_mesh);
            
            inline void assign( int2type<1> )
            {
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
                array_type &aX = X();
                size_t v = 0;
                for(unit_t i=this->lower;i<=this->upper;++i,++v)
                {
                    new (this->pvtx+v) VERTEX(v,aX[i] );
                }
                this->assigned = true;
                
                //______________________________________________________________
                //
                // allocate edges+cells
                //______________________________________________________________
                structured<mesh_type>::assign(*this);
                
            }
            
            inline void assign( int2type<2> )
            {
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
                array_type &aX = X();
                array_type &aY = Y();
                size_t v = 0;
                for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                {
                    for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                    {
                        new (this->pvtx+v) VERTEX(v,aX[j][i],aY[j][i] );
                    }
                }
                this->assigned = true;
                
                //______________________________________________________________
                //
                // allocate edges+cells
                //______________________________________________________________
                structured<mesh_type>::assign(*this);
            }
            
            inline void assign( int2type<3> )
            {
                array_type &aX = X();
                array_type &aY = Y();
                array_type &aZ = Z();
                size_t v = 0;
                for(unit_t k=this->lower.z;k<=this->upper.z;++k)
                {
                    for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                    {
                        for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                        {
                            new (this->pvtx+v) VERTEX(v,aX[k][j][i], aY[k][j][i], aZ[k][j][i] );
                        }
                    }
                }
                this->assigned = true;
                
                //! edges
                const size_t nx        = this->width.x;
                const size_t ny        = this->width.y;
                const size_t nz        = this->width.z;
                const size_t x_edges   = nx - 1;
                const size_t y_edges   = ny - 1;
                const size_t z_edges   = nz - 1;
                const size_t num_edges = nz * (ny * x_edges + nx * y_edges) + (nx*ny) * z_edges;
                this->edges.reserve(num_edges);
                
                
                // X edges
                for(unit_t k=this->lower.z;k<=this->upper.z;++k)
                {
                    for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                    {
                        for(unit_t i=this->lower.x,ip=i+1;i<this->upper.x;++i,++ip)
                        {
                            const coord3D C0(i, j,k);
                            const coord3D C1(ip,j,k);
                            const size_t  I0 = this->offset_of(C0);
                            const size_t  I1 = this->offset_of(C1);
                            assert(I0<this->vertices);
                            assert(I1<this->vertices);
                            const EDGE edge( this->pvtx[I0], this->pvtx[I1] );
                            if( !this->edges.insert(edge) )
                            {
                                this->throw_multiple_edges(I0,I1);
                            }
                            
                        }
                    }
                }
                
                
                // Y edges
                for(unit_t k=this->lower.z;k<=this->upper.z;++k)
                {
                    for(unit_t j=this->lower.y,jp=j+1;j<this->upper.y;++j,++jp)
                    {
                        for(unit_t i=this->lower.x;i<=this->upper.x;++i)
                        {
                            const coord3D C0(i,j,k);
                            const coord3D C1(i,jp,k);
                            const size_t  I0 = this->offset_of(C0);
                            const size_t  I1 = this->offset_of(C1);
                            assert(I0<this->vertices);
                            assert(I1<this->vertices);
                            const EDGE edge( this->pvtx[I0], this->pvtx[I1] );
                            if( !this->edges.insert(edge) )
                            {
                                this->throw_multiple_edges(I0,I1);
                            }
                            
                        }
                    }
                }
                
                // Z edges
                for(unit_t k=this->lower.z,kp=k+1;k<this->upper.z;++k,++kp)
                {
                    for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                    {
                        for(unit_t i=this->lower.x;i<=this->upper.x;++i)
                        {
                            const coord3D C0(i,j,k);
                            const coord3D C1(i,j,kp);
                            const size_t  I0 = this->offset_of(C0);
                            const size_t  I1 = this->offset_of(C1);
                            assert(I0<this->vertices);
                            assert(I1<this->vertices);
                            const EDGE edge( this->pvtx[I0], this->pvtx[I1] );
                            if( !this->edges.insert(edge) )
                            {
                                this->throw_multiple_edges(I0,I1);
                            }
                        }
                    }
                }
                assert(num_edges==this->edges.size());
            }
        };
        
    }
    
}

#endif

