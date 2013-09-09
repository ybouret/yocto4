#ifndef YOCTO_GEMS_LIBRARY_INCLUDED
#define YOCTO_GEMS_LIBRARY_INCLUDED 1

#include "yocto/gems/residue.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class library
        {
        public:
            explicit library();
            virtual ~library() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
    }
    
}


#endif
