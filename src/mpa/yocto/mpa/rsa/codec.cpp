#include "yocto/mpa/rsa/codec.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace mpa
    {
        
        namespace RSA
        {
            
            //__________________________________________________________________
            //
            // Codec
            //__________________________________________________________________
            Codec:: ~Codec() throw()
            {
            }
            
            Codec:: Codec( const Key &k ) :
            pack::q_codec(),
            key( k.clone() ),
            plain(),
            coded()
            {
            }
            
            void Codec:: reset() throw()
            {
                Q.free();
                plain.free();
                coded.free();
            }
            
            //__________________________________________________________________
            //
            // encoder
            //__________________________________________________________________
            encoder:: encoder(const Key &k ) :
            Codec(k)
            {
                
            }
            
            encoder:: ~encoder() throw()
            {
                
            }
            
            void encoder:: write( char C )
            {
                //______________________________________________________________
                //
                // say a char is coming
                //______________________________________________________________
                plain.push( true );
                //______________________________________________________________
                //
                // add some noise
                //______________________________________________________________
                plain.push( memIO::random_bit() );
                //______________________________________________________________
                //
                // write 8 bits
                //______________________________________________________________
                plain.push_full<uint8_t>(C);
                
                //______________________________________________________________
                //
                // check enough bits to write
                //______________________________________________________________
                while(plain.size()>=key->ibits)
                    emit();
            }
            
            void encoder:: emit()
            {
                assert(plain.size()>=key->ibits);
                //__________________________________________________________
                //
                // fetch a message from input bits
                //__________________________________________________________
                const mpn M = mpn::query(plain,key->ibits);
                //__________________________________________________________
                //
                // encode it
                //__________________________________________________________
                const mpn C = key->encode(M); assert(C<key->modulus);
                std::cerr << "Encoding [" << M << "] -> [" << C << "]" << std::endl;

                //__________________________________________________________
                //
                // store in output bits
                //__________________________________________________________
                C.store(coded,key->obits);
                
                //__________________________________________________________
                //
                // convert in output bytes
                //__________________________________________________________
                while(coded.size()>=8)
                    Q.push_back( coded.pop_full<uint8_t>() );
                
                
            }
            
            
            void encoder:: flush()
            {
                //______________________________________________________________
                //
                // say the end is coming
                //______________________________________________________________
                plain.push( false );
                
                //______________________________________________________________
                //
                // fill with noise for at least a new message
                //______________________________________________________________
                while( 0 != (plain.size()%key->ibits) )
                    plain.push( memIO::random_bit() );
                
                //______________________________________________________________
                //
                // emit all the left message(s)
                //______________________________________________________________
                while(plain.size()>=key->ibits)
                    emit();
                assert(0==plain.size());
                
                //______________________________________________________________
                //
                // round output bits to bytes
                //______________________________________________________________
                while( 0 != (coded.size() % 8) )
                    coded.push( memIO::random_bit() );
                
                //______________________________________________________________
                //
                // convert into bytes
                //______________________________________________________________
                while( coded.size()>=8 )
                    Q.push_back( coded.pop_full<uint8_t>() );
                assert(0==coded.size());
            }
            
            //__________________________________________________________________
            //
            // decoder
            //__________________________________________________________________
            decoder:: decoder(const Key &k ) : Codec(k)
            {
                
            }
            
            decoder:: ~decoder() throw()
            {
                
            }
            
            
            void decoder:: write(char chr)
            {
                //______________________________________________________________
                //
                // fill coded bits
                //______________________________________________________________
                coded.push_full<uint8_t>(chr);
                
                
                //______________________________________________________________
                //
                // decode if possible
                //______________________________________________________________
                while( coded.size() >= key->obits )
                {
                    const mpn C = mpn::query(coded,key->obits);
                    if(C>=key->modulus)
                        throw exception("RSA::decoder(Invalid Coded Stream)");
                    const mpn P = key->decode(C);
                    std::cerr << "Decoding [" << C << "] -> [" << P << "]" << std::endl;

                    if(P.bits()>key->ibits)
                        throw exception("RSA::decoder(Invalid Plain Stream)");
                    P.store(plain,key->ibits);
                    
                    //______________________________________________________________
                    //
                    // build bytes
                    //______________________________________________________________
                    while(plain.size()>0)
                    {
                        if(plain.peek())
                        {
                            // a byte
                            if(plain.size()>=10)
                            {
                                plain.pop();                              // flag
                                plain.pop();                              // noise
                                Q.push_back( plain.pop_full<uint8_t>() ); // byte
                                std::cerr << "decoded '" << Q.back() << "'" << std::endl;
                            }
                            else
                                break; // not enough bits
                        }
                        else
                        {
                            // a flush occured
                            exit(0);
                        }
                    }
                    
                    
                }
                            
            }
            
            void decoder:: flush()
            {
                
            }
            
            
        }
    }
}
