#include "yocto/pki/rsa-auth.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace pki
    {
        
        rsa_auth:: ~rsa_auth() throw()
        {
        }
        
        
        rsa_auth:: rsa_auth() throw() :
        plain(),
        coded()
        {
        }
        
        
        string rsa_auth::encrypt( const void *data, size_t size, const rsa_key &key )
        {
            static const size_t max_size = 0xFFFF;
            Random::Bits       &bits     = Random::CryptoBits();
            
            if( size > max_size )
                throw exception("rsa_auth::encrypt(size overflow)");
            
            plain.free();
            coded.free();
            //! store the size
            plain.push_full<uint16_t>( size );
            
            //! store the key
            const uint8_t *p = (const uint8_t *)data;
            for( size_t i=0; i < size; ++i )
                plain.push_full<uint8_t>( p[i] );
            
            //! pad with noise
            while( 0 != ( plain.size() % key.ibits ) ) plain.push( bits() );
            
            //! encrypt!
            while( plain.size() > 0 )
            {
                assert(plain.size()>=key.ibits);
                const natural P = natural::query( plain, key.ibits );
                const natural C = key.compute( P );
                C.store( coded, key.obits );
            }
            
            //! pad to 8 bits
            while( coded.size() & 7 ) coded.push_back( bits() );
            
            string ans( coded.size() >> 3, as_capacity );
            while( coded.size() > 0 )
            {
                assert( coded.size() >= 8 );
                ans.append( coded.pop_full<uint8_t>() );
            }
            return ans;
        }
        
    }
    
}
