#ifndef YOCTO_MESH_INCLUDED
#define YOCTO_MESH_INCLUDED 1

#include "yocto/swamp/arrays.hpp"

namespace yocto 
{
    
    namespace swamp 
    {
        
        class mesh : public object
        {
        public:
            virtual ~mesh() throw();
            
        protected:
            explicit mesh( array_db &db ) throw();
            array_db &adb;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mesh);
        };
    }
    
}


#endif
