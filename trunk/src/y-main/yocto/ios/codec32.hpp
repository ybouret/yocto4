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
        
#define Y_CODEC32_ARGS  \
void          *destbuf, \
uint32_t      &destlen, \
const void    *srcbuf,  \
const uint32_t srclen,  \
void          *options
        
#define Y_CODEC32_ARGS_NO_OPT \
void          *destbuf, \
uint32_t      &destlen, \
const void    *srcbuf,  \
const uint32_t srclen,  \
void          *
        
#define Y_CODEC32_CHECK() do { assert(!(destbuf==0&&destlen>0)); assert(!(srcbuf==0&&srclen>0)); } while(false)
        
#define Y_CODEC32_DECL() virtual void encode(Y_CODEC32_ARGS); virtual void decode(Y_CODEC32_ARGS)
        
        
        class identity32 : public codec32
        {
        public:
            explicit identity32() throw();
            virtual ~identity32() throw();
            
            Y_CODEC32_DECL();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(identity32);
        };
    }
}

#endif
