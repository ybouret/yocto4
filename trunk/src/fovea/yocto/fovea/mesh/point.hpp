#ifndef YOCTO_FOVEA_POINT_MESH_INCLUDED
#define YOCTO_FOVEA_POINT_MESH_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/arrays.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <size_t DIM, typename T>
        class point_mesh : public mesh_of<T>, public layout1D
        {
        public:
            typedef array1D<T>                       axis_type;
            typedef mesh_of<T> mesh_type;
            
            virtual ~point_mesh() throw() {}
            
            explicit point_mesh(array_db       &a,
                                const layout1D &L) :
            mesh_type(a,DIM,L.items,mesh::is_point),
            layout1D(L)
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string    axis_n = this->axis_name(i);
                    this->adb.store( new axis_type(axis_n,*this) );
                }
            }
            
            inline axis_type       &X()       { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline axis_type       &Y()       { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline axis_type       &Z()       { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(point_mesh);
        };
    }
}
#endif
