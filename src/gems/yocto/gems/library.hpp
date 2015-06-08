#ifndef YOCTO_GEMS_LIBRARY_INCLUDED
#define YOCTO_GEMS_LIBRARY_INCLUDED 1

#include "yocto/gems/properties.hpp"

namespace yocto
{
    namespace gems
    {
        class library : public properties::db
        {
        public:
            explicit library() throw();
            virtual ~library() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
    }
}

#endif
