#ifndef YOCTO_MPL_RSA_CODEC_INCLUDED
#define YOCTO_MPL_RSA_CODEC_INCLUDED 1

#include "yocto/mpl/rsa/keys.hpp"
#include "yocto/pack/q-codec.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace mpl
    {

        namespace RSA
        {
            class Codec : public pack::q_codec
            {
            public:
                static const size_t W = 10;
                virtual ~Codec() throw();

                virtual void reset() throw();

            protected:
                explicit Codec(const Key & k);
                auto_ptr<Key> key;
                ios::bitio    plain;
                ios::bitio    coded;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Codec);
            };


            class encoder : public Codec
            {
            public:
                explicit encoder(const Key &k);
                virtual ~encoder() throw();

                //! write 10 bits to plain
                /**
                 - a 1 flag bit => there is a char
                 - a random bit => add some noise
                 - the eight bits of the car C
                 */
                virtual void write( char C );

                //! write flushing bits to plain
                /**
                 - a 0 flag bit => stop
                 - some random bits to be modulo key->ibits
                 */
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

                //! write and test for obits
                virtual void write( char C );

                virtual void flush();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(decoder);
            };


        }
    }
}

#endif
