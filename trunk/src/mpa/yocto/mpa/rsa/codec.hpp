#ifndef YOCTO_MPA_RSA_CODEC_INCLUDED
#define YOCTO_MPA_RSA_CODEC_INCLUDED 1

#include "yocto/mpa/rsa/keys.hpp"
#include "yocto/pack/q-codec.hpp"
#include "yocto/ios/bitio.hpp"

namespace yocto
{
    namespace mpa
    {
        
        namespace RSA
        {
            class Codec : public pack::q_codec
            {
            public:
                virtual ~Codec() throw();
                
            protected:
                explicit Codec();
                ios::bitio bio;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Codec);
            };
        }
    }
}

#endif
