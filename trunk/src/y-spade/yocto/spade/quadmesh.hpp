#ifndef YOCTO_SPADE_QUADMESH_INCLUDED
#define YOCTO_SPADE_QUADMESH_INCLUDED 1


#include "yocto/spade/array-db.hpp"


namespace yocto
{
    namespace spade
    {
        
        class quadmesh
        {
        public:
            virtual ~quadmesh() throw();
            
            static const char *get_axis_name( dimension_t dim ) throw();
            
        protected:
            explicit quadmesh( array_db &adb_ref ) throw();
            array_db &adb;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quadmesh);
        };
    }
    
}



#endif
