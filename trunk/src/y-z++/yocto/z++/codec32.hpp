#ifndef YOCTO_ZLIB_CODEC32_INCLUDED
#define YOCTO_ZLIB_CODEC32_INCLUDED 1

#include "yocto/ios/codec32.hpp"

namespace yocto
{
    namespace zlib
    {
        
        class codec32 : public ios::codec32
        {
        public:
            explicit codec32() throw();
            virtual ~codec32() throw();
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(codec32);
        };
        
    }
}

#endif