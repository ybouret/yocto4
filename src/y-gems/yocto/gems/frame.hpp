#ifndef YOCTO_GEMS_FRAME_INCLUDED
#define YOCTO_GEMS_FRAME_INCLUDED 1

#include "yocto/gems/library.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename T>
        class frame 
        {
        public:
            virtual ~frame() throw();
            explicit frame();
        
        private:
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(frame);
        public:
            library<T> lib;
        };
        
    }
}

#endif
