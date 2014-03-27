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
            typedef typename array_of<LAYOUT::DIMENSIONS,T>::type array_type;
            
            inline explicit curvilinear_mesh( array_db &a, const LAYOUT &L ) :
            mesh(a,
                 LAYOUT::DIMENSIONS,
                 mesh::is_curvilinear,
                 mesh::sz2fp( sizeof(T) )
                 )
            {
                for(size_t i=0;i<dimensions;++i)
                {
                    const string array_n = mesh::axis_name(i);
                    adb.store( new array_type(array_n, *this) );
                }
            }
            
            inline virtual ~curvilinear_mesh() throw()
            {
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(curvilinear_mesh);
        };
        
    }
    
}

#endif

