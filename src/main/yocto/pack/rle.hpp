#ifndef YOCTO_PACK_RLE_INCLUDED
#define YOCTO_PACK_RLE_INCLUDED 1

#include "yocto/pack/q-codec.hpp"

namespace yocto
{
    namespace pack
    {
        
        struct rle
        {
            
            class encoder : public pack::q_codec
            {
            public:
                explicit encoder() throw();
                virtual ~encoder() throw();
                
                virtual void reset() throw();
                
                virtual void write(char C);
                virtual void flush();
                
                size_t compressed_size( const void *buffer, const size_t buflen );
                
            private:
                int    chr; //!< -1: starting, [0:255] already read
                size_t num; //!< number of extra current, max=255
                void   emit();
                YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
            };
            
            class decoder : public pack::q_codec
            {
            public:
                explicit decoder() throw();
                virtual ~decoder() throw();
                
                virtual void reset() throw();
                
                virtual void write(char C);
                virtual void flush();
                
                
            private:
                enum status
                {
                    wait_for_first,
                    wait_for_second,
                    wait_for_count
                };
                status flag;
                int    chr;
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(decoder);
            };
            
            
        };
        
    }
}

#endif

