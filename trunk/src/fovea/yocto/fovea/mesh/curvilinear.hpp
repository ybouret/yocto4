#ifndef YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/arrays.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class curvilinear_mesh : public mesh, public LAYOUT
        {
        public:
            typedef typename array_of<LAYOUT::DIMENSIONS,T>::type   array_type;
            typedef typename array_of<LAYOUT::DIMENSIONS,T>::vertex vertex;

            inline explicit curvilinear_mesh( array_db &a, const LAYOUT &L ) :
            mesh(a,
                 LAYOUT::DIMENSIONS,
                 L.items,
                 mesh::is_curvilinear,
                 sizeof(T)
                 ),
            LAYOUT(L)
            {
                for(size_t i=0;i<dims;++i)
                {
                    const string array_n = mesh::axis_name(i);
                    adb.store( new array_type(array_n, *this) );
                }
            }
            
            inline virtual ~curvilinear_mesh() throw()
            {
            }
            
            inline array_type       &X()       { return adb[ mesh::axis_name(0) ].template as<array_type>(); }
            inline const array_type &X() const { return adb[ mesh::axis_name(0) ].template as<array_type>(); }
            inline array_type       &Y()       { return adb[ mesh::axis_name(1) ].template as<array_type>(); }
            inline const array_type &Y() const { return adb[ mesh::axis_name(1) ].template as<array_type>(); }
            inline array_type       &Z()       { return adb[ mesh::axis_name(2) ].template as<array_type>(); }
            inline const array_type &Z() const { return adb[ mesh::axis_name(2) ].template as<array_type>(); }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(curvilinear_mesh);
        };
        
    }
    
}

#endif

