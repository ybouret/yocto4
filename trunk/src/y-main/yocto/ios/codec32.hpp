#ifndef YOCTO_IOS_CODEC32_INCLUDED
#define YOCTO_IOS_CODEC32_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace ios
    {
        class codec32
        {
        public:
            virtual ~codec32() throw();
            
            virtual void encode(void          *destbuf,
                                uint32_t      &destlen,
                                const void    *srcbuf,
                                const uint32_t srclen,
                                void          *options) = 0;
            
            virtual void decode(void          *destbuf,
                                uint32_t      &destlen,
                                const void    *srcbuf,
                                const uint32_t srclen,
                                void          *options) = 0;
            
        protected:
            explicit codec32() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(codec32);
        };
    }
}

#endif
