#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gems
    {
        
        class properties : public object, public vslot
        {
        public:
            
            virtual ~properties() throw();
            explicit properties() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };
     
        
        
    }
    
}



#endif
