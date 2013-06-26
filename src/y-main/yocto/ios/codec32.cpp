#include "yocto/ios/codec32.hpp"
#include "yocto/exception.hpp"

#include <cstring>

namespace yocto
{
    namespace ios
    {
        
        codec32:: ~codec32() throw() {}
        codec32::  codec32() throw() {}
        
        
        identity32:: identity32() throw() {}
        
        identity32:: ~identity32() throw() {}
        
        void identity32:: encode( Y_CODEC32_ARGS_NO_OPT )
        {
            Y_CODEC32_CHECK();
            if(destlen<srclen)
                throw exception("identity32.encode(destlen=%u<srclen=%u)", unsigned(destlen), unsigned(srclen) );
            memcpy(destbuf,srcbuf,destlen=srclen);
        }
        
        void identity32:: decode( Y_CODEC32_ARGS_NO_OPT )
        {
            Y_CODEC32_CHECK();
            if(destlen<srclen)
                throw exception("identity32.decode(destlen=%u<srclen=%u)", unsigned(destlen), unsigned(srclen) );
            memcpy(destbuf,srcbuf,destlen=srclen);
        }


    }
}
