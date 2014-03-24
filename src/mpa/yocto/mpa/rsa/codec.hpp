#ifndef YOCTO_MPA_RSA_CODEC_INCLUDED
#define YOCTO_MPA_RSA_CODEC_INCLUDED 1

#include "yocto/mpa/rsa/keys.hpp"
#include "yocto/pack/q-codec.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace mpa
    {
        
        namespace RSA
        {
            class Codec : public pack::q_codec
            {
            public:
                static const size_t rbits = 1; //!< add random bit
                virtual ~Codec() throw();
                
                virtual void reset() throw();
                
            protected:
                explicit Codec(const Key & k);
                auto_ptr<Key> key;
                ios::bitio    I;
                ios::bitio    O;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Codec);
            };
            
            class encoder : public Codec
            {
            public:
                explicit encoder(const Key &k);
                virtual ~encoder() throw();
                
                virtual void write( char C );
                virtual void flush();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
                void emit();
            };
            
            
            class decoder : public Codec
            {
            public:
                explicit decoder(const Key &k);
                virtual ~decoder() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(decoder);
            };

            
        }
    }
}

#endif
