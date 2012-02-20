#include "yocto/pki/rsa-auth.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/random/bits.hpp"

namespace yocto
{
    
    namespace pki
    {
        
        rsa_auth:: ~rsa_auth() throw()
        {
        }
        
        
        rsa_auth:: rsa_auth( const rsa_private_key &prv ) :
        prv_k( new rsa_private_key(prv) ),
        pub_k( new rsa_public_key(prv)  ),
        plain(),
        coded()
        {
        }
        
        
        string rsa_auth::encrypt( const void *data, size_t size, hashing::function &h )
        {
            Random::Bits &bits = Random::CryptoBits();
            assert( prv_k->modulus == pub_k->modulus );
            h.set();
            plain.free();
            coded.free();
            
            //! aligned input bits
            size_t data_bits    = size*8;
            while( 0 != ( data_bits % prv_k->ibits ) ) ++data_bits;
            
            //! aligned input mpk::natural
            const size_t data_units   = data_bits  / prv_k->ibits;
            
            //! aligned code bits
            size_t       code_bits    = data_units * prv_k->obits;
            while( 0 != ( code_bits & 7 ) ) ++code_bits;
            
            //! output bytes for data
            const size_t code_bytes   = code_bits / 8;
            
            //-- fill plain bits with data
            {
                const uint8_t *p = (uint8_t*)data;
                for( size_t i=0; i < size; ++i )
                {
                    plain.push_full<uint8_t>( p[i] );
                }
            }
            
            //-- padd with noise
            while( plain.size() < data_bits ) plain.push( bits() );

            //-- 
            //const size_t hash_bytes   = h.length;
            
            
            
            
            return string();
        }
        
    }
    
}
