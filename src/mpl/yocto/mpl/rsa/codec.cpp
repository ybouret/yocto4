#include "yocto/mpl/rsa/codec.hpp"

namespace yocto
{
    namespace mpl
    {

        namespace RSA
        {


            Codec:: Codec(const Key &k) :
            key( k.clone() ),
            plain(),
            coded()
            {
            }

            Codec:: ~Codec() throw()
            {
            }

            void Codec:: reset() throw()
            {
                clear();
                plain.free();
                coded.free();
            }

            
        }
    }
}


namespace yocto
{
    namespace mpl
    {

        namespace RSA
        {

            encoder:: encoder(const Key &k) :
            Codec(k)
            {
            }

            encoder:: ~encoder() throw()
            {
            }

            void encoder::write(char C)
            {
                plain.push(true);
                plain.push(manager::random_bit());
                plain.push_full<uint8_t>(C);
                emit();
            }

            void encoder:: flush()
            {
                plain.push(false);
                const size_t m = key->ibits;
                while( 0 !=( plain.size() % m ) )
                {
                    plain.push(manager::random_bit());
                }
            }

            void encoder::emit()
            {
                const size_t ibits = key->ibits;
                const size_t obits = key->obits;
                while( plain.size() >= ibits )
                {
                    // get ibits message
                    const natural M = natural::get(plain,ibits);

                    // encode it
                    const natural C = key->encode(M);

                    // put obits message
                    C.put(coded,obits);
                }
            }

        }
    }
}

