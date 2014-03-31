#ifndef YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/array1d.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class rectilinear_mesh : public Mesh<LAYOUT::DIMENSIONS,T>, public LAYOUT
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef array1D<T>                    axis_type;
            typedef Mesh<LAYOUT::DIMENSIONS,T>    MESH;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            
            const LAYOUT & get_layout() const throw() { return *this; }
            
            
            //__________________________________________________________________
            //
            // initialization
            //__________________________________________________________________
            inline explicit rectilinear_mesh(array_db     &a,
                                             const LAYOUT &L ) :
            MESH(a,L.items,mesh::is_rectilinear),
            LAYOUT(L)
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string    axis_n = this->axis_name(i);
                    const layout1D  axis_l( __coord(this->lower,i), __coord(this->upper,i));
                    this->adb.store( new axis_type(axis_n,axis_l) );
                }
                assign( int2type<LAYOUT::DIMENSIONS>() );
                this->assigned = true;
            }
            
            inline virtual ~rectilinear_mesh() throw() {}
            
            //__________________________________________________________________
            //
            // get axis
            //__________________________________________________________________
            inline axis_type       &X()       { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline axis_type       &Y()       { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline axis_type       &Z()       { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rectilinear_mesh);
            inline void assign( int2type<1> ) throw()
            {
                // vertices
                axis_type &aX = X();
                size_t v = 0;
                for(unit_t i=this->lower;i<=this->upper;++i,++v)
                {
                    new (this->vtx+v) VERTEX(v,aX[i] );
                }
                
                // edges
                const size_t edges = this->vertices - 1;
                this->edb.reserve(edges);
                for(size_t i1=0,i2=1;i1<edges;++i1,++i2)
                {
                    const EDGE edge( this->vtx[i1], this->vtx[i2] );
                    if( !this->edb.insert(edge) )
                    {
                        this->throw_multiple_edges(i1,i2);
                    }
                }
                assert( this->edb.size() == edges );
            }
            
            inline void assign( int2type<2> ) throw()
            {
                axis_type &aX = X();
                axis_type &aY = Y();
                
                //! vertices
                // vertex index: (i-lower.x) + witdh.x * (j-lower.y)
                size_t v = 0;
                for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                {
                    T &y = aY[j];
                    for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                    {
                        new (this->vtx+v) VERTEX(v,aX[i], y );
                    }
                }
                
                //! edges
                const size_t nx      = this->width.x;
                const size_t ny      = this->width.y;
                const size_t x_edges = nx - 1;
                const size_t y_edges = ny - 1;
                const size_t edges   = ny * x_edges + nx * y_edges;
                this->edb.reserve(edges);
                
                //! x edges
                for(size_t j=0;j<ny;++j)
                {
                    const size_t joff = j * nx;
                    for(size_t i1=0,i2=1;i1<x_edges;++i1,++i2)
                    {
                        const size_t I1 = i1+joff;
                        const size_t I2 = i2+joff;
                        const EDGE edge( this->vtx[I1], this->vtx[I2] );
                        if( !this->edb.insert(edge) )
                        {
                            this->throw_multiple_edges(I1,I2);
                        }
                    }
                }
                
                //! y edges
                for(size_t i=0;i<nx;++i)
                {
                    for(size_t j1=0,j2=1;j1<y_edges;++j1,++j2)
                    {
                        const size_t I1 = i + (j1*nx);
                        const size_t I2 = i + (j2*nx);
                        const EDGE edge( this->vtx[I1], this->vtx[I2] );
                        if( !this->edb.insert(edge) )
                        {
                            this->throw_multiple_edges(I1,I2);
                        }
                    }
                }
                assert( this->edb.size() == edges );
            }
            
            inline void assign( int2type<3> ) throw()
            {
                axis_type &aX = X();
                axis_type &aY = Y();
                axis_type &aZ = Z();
                size_t v = 0;
                for(unit_t k=this->lower.z;k<=this->upper.z;++k)
                {
                    T &z = aZ[k];
                    for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                    {
                        T &y = aY[j];
                        for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                        {
                            new (this->vtx+v) VERTEX(v,aX[i], y,z );
                        }
                    }
                }
                
                //! edges
                const size_t nx      = this->width.x;
                const size_t ny      = this->width.y;
                const size_t nz      = this->width.z;
                const size_t x_edges = nx - 1;
                const size_t y_edges = ny - 1;
                const size_t z_edges = nz - 1;
                const size_t edges   = nz * (ny * x_edges + nx * y_edges) + (nx*ny) * z_edges;
                this->edb.reserve(edges);
                
                
                //! x edges
                const size_t nxy = nx*ny;
                for(size_t k=0;k<nz;++k)
                {
                    const size_t koff = k * nxy;
                    for(size_t j=0;j<ny;++j)
                    {
                        const size_t joff = j * nx + koff;
                        for(size_t i1=0,i2=1;i1<x_edges;++i1,++i2)
                        {
                            const size_t I1 = i1+joff;
                            const size_t I2 = i2+joff;
                            const EDGE edge( this->vtx[I1], this->vtx[I2] );
                            if( !this->edb.insert(edge) )
                            {
                                this->throw_multiple_edges(I1,I2);
                            }
                        }
                    }
                }
                
            }
            
        };
        
        
    }
    
}

#endif

