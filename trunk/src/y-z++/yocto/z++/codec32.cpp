#include "yocto/z++/codec32.hpp"
#include "yocto/zlib/zlib.h"

#include <iostream>

namespace yocto
{
    namespace zlib
    {
        codec32:: codec32() throw() {}
        
        codec32:: ~codec32() throw() {}
        
        
        int32_t codec32:: Encode(void           *destbuf,
                                 uint32_t       &destlen,
                                 const void     *srcbuf,
                                 const uint32_t  srclen,
                                 const int32_t   level) throw()
        {
            
            Y_CODEC32_CHECK();
            
            Bytef       *target    = (Bytef *)destbuf;
            uLong        targetLen = destlen;
            const Bytef *source    = (const Bytef *)srcbuf;
            const uLong  sourceLen = srclen;
           
            const int    ans = compress2(target, &targetLen, source, sourceLen, int(level));
            if(ans!=Z_OK)
                return 1;
            
            destlen = targetLen;
            return 0;
        }
        
        uint32_t codec32:: EncodeSpace(uint32_t src) throw()
        {
            return compressBound(src);
        }
        
        void codec32:: encode( Y_CODEC32_ARGS )
        {
        }
        
        void codec32:: decode( Y_CODEC32_ARGS_NO_OPT )
        {
            
        }
        
    }
    
}

