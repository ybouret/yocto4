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
            
            Y_CODEC32_DECL();
            
            //! need a memory lock before call
            static int32_t Encode(void           *destbuf,
                                  uint32_t       &destlen,
                                  const void     *srcbuf,
                                  const uint32_t  srclen,
                                  const int       level) throw();
            
            static uint32_t EncodeSpace(uint32_t src) throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(codec32);
        };
        
    }
}

#endif

