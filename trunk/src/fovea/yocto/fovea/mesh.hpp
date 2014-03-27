#ifndef YOCTO_FOVEA_MESH_INCLUDED
#define YOCTO_FOVEA_MESH_INCLUDED 1

#include "yocto/fovea/dimensions.hpp"
#include "yocto/fovea/array-db.hpp"

namespace yocto
{
    
    namespace fovea
    {
     
        
        class mesh
        {
        public:
            enum real_type
            {
                use_float,  //!< use float vertices
                use_double  //!< use double vertices
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

            static real_type   sz2fp( const unsigned sz );
            static const char *axis_name( size_t dim );
            
        protected:
            array_db &adb;
            explicit mesh(array_db       &a,
                          const size_t    d,
                          const form_type f,
                          const real_type r) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mesh);
        };
        
    }
    
}

#endif
