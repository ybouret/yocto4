#ifndef YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/vertex.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class curvilinear_mesh : public mesh_of<T>, public LAYOUT
        {
        public:
            typedef typename types_for<LAYOUT::DIMENSIONS,T>::array_type  array_type;
            typedef mesh_of<T> mesh_type;
            
            inline explicit curvilinear_mesh( array_db &a, const LAYOUT &L ) :
            mesh_type(a,
                      LAYOUT::DIMENSIONS,
                      L.items,
                      mesh::is_curvilinear),
            LAYOUT(L)
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string array_n = this->axis_name(i);
                    this->adb.store( new array_type(array_n, *this) );
                }
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
        };
        
    }
    
}

#endif

