#ifndef YOCTO_MK_SIG_SMOOTHER_INCLUDED
#define YOCTO_MK_SIG_SMOOTHER_INCLUDED 1

#include "yocto/math/sig/extender.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename>
        class smoother
        {
        public:
            explicit smoother();
            virtual ~smoother() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(smoother);
        };
        
    }
}

#endif


