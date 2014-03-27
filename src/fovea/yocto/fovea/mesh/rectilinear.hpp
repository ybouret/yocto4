#ifndef YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/arrays.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class rectilinear_mesh : public mesh, public LAYOUT
        {
        public:
            typedef array1D<T>                                      axis_type;
            typedef typename array_of<LAYOUT::DIMENSIONS,T>::vertex vertex;

            
            inline explicit rectilinear_mesh(array_db     &a,
                                             const LAYOUT &L ) :
            mesh(a,
                 LAYOUT::DIMENSIONS,
                 L.items,
                 mesh::is_rectilinear,
                 sizeof(T) ),
            LAYOUT(L)
            {
                for(size_t i=0;i<dims;++i)
                {
                    const string    axis_n = axis_name(i);
                    const layout1D  axis_l( __coord(this->lower,i), __coord(this->upper,i));
                    a.store( new axis_type(axis_n,axis_l) );
                }
            }
            
            inline virtual ~rectilinear_mesh() throw() {}
            
            inline axis_type       &X()       { return adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline axis_type       &Y()       { return adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline axis_type       &Z()       { return adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rectilinear_mesh);
        };
        
        
    }
    
}

#endif

