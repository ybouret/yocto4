#ifndef YOCTO_MESH_INCLUDED
#define YOCTO_MESH_INCLUDED 1

#include "yocto/swamp/arrays.hpp"

namespace yocto 
{
    
    namespace swamp 
    {
        
        class quadmesh : public object
        {
        public:
            virtual ~quadmesh() throw();
            
            static const char *axis_name( size_t iAxis );
            static const char *delta_name( size_t iAxis );
            
        protected:
            explicit quadmesh( array_db &db ) throw();
            array_db &adb;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quadmesh);
        };
    }
    
}


#endif
