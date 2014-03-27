#ifndef YOCTO_FOVEA_MESH_INCLUDED
#define YOCTO_FOVEA_MESH_INCLUDED 1

#include "yocto/fovea/dimensions.hpp"

namespace yocto
{
    
    namespace fovea
    {
     
        
        class mesh
        {
        public:
            enum real_type
            {
                use_real32, //!< use float vertices
                use_real64  //!< use double vertices
            };
            enum form_type
            {
                is_rectilinear,
                is_curvilinear
            };
            
            virtual ~mesh() throw();
            
            const size_t    dimensions;
            const form_type form;
            const real_type real;

        protected:
            explicit mesh(const size_t    d,
                          const form_type f,
                          const real_type r) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mesh);
        };
        
    }
    
}

#endif
