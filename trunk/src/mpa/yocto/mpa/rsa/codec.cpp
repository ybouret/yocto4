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
            I(),
            O()
            {
            }
            
            void Codec:: reset() throw()
            {
                Q.free();
                I.free();
                O.free();
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
                I.push( true );                    // byte
                for(size_t i=rbits;i>0;--i)        //
                    I.push( memIO::random_bit() ); // rand
                I.push_full<uint8_t>(C);
                if(I.size()>=key->ibits)
                    emit();
            }
            
            void encoder:: emit()
            {
                assert(I.size()>=key->ibits);
                while(I.size()>=key->ibits)
                {
                    const mpn M = mpn::query(I,key->ibits);
                    const mpn C = key->encode(M); assert(C<key->modulus);
                    C.store(O,key->obits);
                    while(O.size()>=8)
                        Q.push_back( O.pop_full<uint8_t>() );
                }
                
            }
            
            
            void encoder:: flush()
            {
                I.push( false );                     // END
                while( 0 != (I.size()%key->ibits) )  //
                    I.push( memIO::random_bit() );   // noise
                emit();                              // encode
                while( 0 != (O.size() % 8) )         //
                    O.push( memIO::random_bit() );   // noise
                while( O.size()>=8 )
                    Q.push_back( O.pop_full<uint8_t>() );
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
            
            
            void decoder:: write(char C)
            {
                I.push_full<uint8_t>(C); // load Input
                if(I.size()>=key->obits)
                    emit();
            }
            
            
            void decoder:: flush()
            {
                
            }
            
            void decoder:: emit()
            {
                while(I.size()>=key->obits)
                {
                    const mpn C = mpn::query(I,key->obits);
                    const mpn P = key->decode(C);
                    if(P.bits()>key->ibits)
                        throw exception("RSA::decoder(corrupted input)");
                    P.store(O,key->ibits);
                }
            }
            
        }
    }
}
