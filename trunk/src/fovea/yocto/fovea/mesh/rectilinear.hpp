#ifndef YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class rectilinear_mesh : public mesh, public LAYOUT
        {
            
        public:
            explicit rectilinear_mesh( array_db &a, const LAYOUT &L ) :
            mesh(a,LAYOUT::DIMENSIONS, mesh::is_rectilinear, mesh::sz2fp(sizeof(T)) )
            {
            }
            
            virtual ~rectilinear_mesh() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rectilinear_mesh);
        };
        
        
    }
    
}

#endif

