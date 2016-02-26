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
                // put a stop
                plain.push(false);

                // round to ibits
                const size_t m = key->ibits;
                while( 0 !=( plain.size() % m ) )
                {
                    plain.push(manager::random_bit());
                }

                // emit all words
                emit();

                // pad final
                while( 0 != (coded.size()&7) )
                {
                    coded.push( manager::random_bit() );
                }

                // write padding
                while(coded.size()>=8)
                {
                    q_codec::store( coded.pop_full<uint8_t>() );
                }
                assert(0==coded.size());
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
                    const natural C = key->encode(M); assert(C.bits()<=obits);
                    assert(C<=key->modulus);
                    std::cerr << "emit:C=" << C << std::endl;
                    // put obits message
                    C.put(coded,obits);
                }
                while(coded.size()>=8)
                {
                    q_codec::store( coded.pop_full<uint8_t>() );
                }
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

            decoder:: decoder(const Key &k) :
            Codec(k)
            {
            }

            decoder:: ~decoder() throw()
            {
            }


            void decoder::write(char C)
            {
                coded.push_full<uint8_t>(C);
                const size_t ibits = key->ibits;
                const size_t obits = key->obits;

                while(coded.size()>=obits)
                {
                    const natural C = natural::get(coded,obits); std::cerr << "read:C=" << C << std::endl;
                    const natural P = key->decode(C);
                    P.put(plain,ibits);
                }

                // extract words
                while(plain.size()>=W)
                {
                    const bool flag = plain.pop();
                    if(!flag)
                    {
                        // cleanup and return;
                        return;
                    }
                    else
                    {
                        // discard random
                        (void) plain.pop();

                        // get value
                        q_codec::store( plain.pop_full<uint8_t>() );
                    }
                }

            }

            void decoder:: flush()
            {

            }


        }

    }

}

