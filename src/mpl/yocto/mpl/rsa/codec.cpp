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
                //______________________________________________________________
                //
                // put a data flag
                //______________________________________________________________
                plain.push(true);

                //______________________________________________________________
                //
                // put some noise
                //______________________________________________________________
                plain.push(manager::random_bit());

                //______________________________________________________________
                //
                // write input bits
                //______________________________________________________________
                plain.push_full<uint8_t>(C);

                //______________________________________________________________
                //
                // process
                //______________________________________________________________
                emit();
            }

            void encoder:: flush()
            {
                //______________________________________________________________
                //
                // put a stop
                //______________________________________________________________
                plain.push(false);

                //______________________________________________________________
                //
                // round to ibits
                //______________________________________________________________
                const size_t m = key->ibits;
                while( 0 !=( plain.size() % m ) )
                {
                    plain.push(manager::random_bit());
                }

                //______________________________________________________________
                //
                // emit all words
                //______________________________________________________________
                emit();

                //______________________________________________________________
                //
                // pad final
                //______________________________________________________________
                while( 0 != (coded.size()&7) )
                {
                    coded.push( manager::random_bit() );
                }

                //______________________________________________________________
                //
                // write padding
                //______________________________________________________________
                while(coded.size()>=8)
                {
                    const uint8_t B = coded.pop_full<uint8_t>();
                    Q.push_back(B);
                }

                assert(0==coded.size());
            }

            void encoder::emit()
            {
                const size_t ibits = key->ibits;
                const size_t obits = key->obits;

                //______________________________________________________________
                //
                // extract ibits chunks
                //______________________________________________________________
                while( plain.size() >= ibits )
                {
                    //__________________________________________________________
                    //
                    // get ibits message
                    //__________________________________________________________
                    const natural M = natural::get(plain,ibits);

                    //__________________________________________________________
                    //
                    // encode it
                    //
                    //__________________________________________________________
                    const natural C = key->encode(M);

                    //__________________________________________________________
                    //
                    // put obits message
                    //__________________________________________________________
                    C.put(coded,obits);
                }

                //______________________________________________________________
                //
                // tranfer bytes
                //______________________________________________________________
                while(coded.size()>=8)
                {
                    const uint8_t B = coded.pop_full<uint8_t>();
                    Q.push_back(B);
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
                //______________________________________________________________
                //
                // write bits to coded stream
                //______________________________________________________________
                const uint8_t B = uint8_t(C);
                coded.push_full<uint8_t>(B);

                //______________________________________________________________
                //
                // extract obits chunks
                //______________________________________________________________
                const size_t ibits = key->ibits;
                const size_t obits = key->obits;

                while(coded.size()>=obits)
                {
                    const natural C = natural::get(coded,obits);
                    const natural P = key->decode(C);
                    P.put(plain,ibits);
                }

                //______________________________________________________________
                //
                // extract bytes from plain stream
                //______________________________________________________________
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
                        plain.skip();

                        // get value
                        Q.push_back( plain.pop_full<uint8_t>() );
                    }
                }

            }

            void decoder:: flush()
            {

            }


        }

    }

}

