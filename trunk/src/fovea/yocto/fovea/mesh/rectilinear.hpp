#ifndef YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/edge.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class rectilinear_mesh : public mesh_of<LAYOUT::DIMENSIONS,T>, public LAYOUT
        {
        public:
            typedef array1D<T>                                            axis_type;
            typedef mesh_of<LAYOUT::DIMENSIONS,T>                         mesh_type;
            typedef typename types_for<LAYOUT::DIMENSIONS,T>::edge_type   edge_type;
            typedef typename mesh_type::VTX                               VTX;
            inline explicit rectilinear_mesh(array_db     &a,
                                             const LAYOUT &L ) :
            mesh_type(a,L.items,mesh::is_rectilinear),
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
                axis_type &aX = X();
                size_t v = 0;
                for(unit_t i=this->lower;i<=this->upper;++i,++v)
                {
                    new (this->vtx+v) VTX(v,aX[i] );
                }
                
            }
            
            inline void assign( int2type<2> ) throw()
            {
                axis_type &aX = X();
                axis_type &aY = Y();
                size_t v = 0;
                for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                {
                    T &y = aY[j];
                    for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                    {
                        new (this->vtx+v) VTX(v,aX[i], y );
                    }
                }
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
                            new (this->vtx+v) VTX(v,aX[i], y,z );
                        }
                    }
                }
            }
            
        };
        
        
    }
    
}

#endif

