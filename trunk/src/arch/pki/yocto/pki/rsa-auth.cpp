#include "yocto/pki/rsa-auth.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/exception.hpp"

#include <iostream>

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
        
        rsa_auth:: rsa_auth( size_t nbits ) :
        plain(),
        coded()
        {
            plain.reserve(nbits);
            coded.reserve(nbits);
        }
        
        string rsa_auth:: encrypt( const memory::ro_buffer &buff, const rsa_key &key )
        {
            return encrypt( buff.ro(), buff.length(), key);
        }
        
        
        void rsa_auth:: reset() throw()
        {
            plain.free();
            coded.free();
        }
        
        string rsa_auth::encrypt( const void *data, size_t size, const rsa_key &key )
        {
            static const size_t max_size = 0xFFFF;
            Random::Bits       &bits     = Random::CryptoBits();
            
            assert( !(NULL==data&&size>0));
            
            if( size > max_size )
                throw exception("rsa_auth::encrypt(size overflow)");
            
            reset();
            
            //------------------------------------------------------------------
            //! store the size
            //------------------------------------------------------------------
            plain.push_full<uint16_t>( size );
            
            //------------------------------------------------------------------
            // store the noise
            //------------------------------------------------------------------
            for( size_t i=0; i < header; ++i )
                plain.push( bits() );
            
            //------------------------------------------------------------------
            //! store the key
            //------------------------------------------------------------------
            const uint8_t *p = (const uint8_t *)data;
            for( size_t i=0; i < size; ++i )
                plain.push_full<uint8_t>( p[i] );
            
            //------------------------------------------------------------------
            //! pad with noise
            //------------------------------------------------------------------
            while( 0 != ( plain.size() % key.ibits ) ) plain.push( bits() );
            
            //------------------------------------------------------------------
            //! encrypt!
            //------------------------------------------------------------------
            while( plain.size() > 0 )
            {
                assert(plain.size()>=key.ibits);
                const natural P = natural::query( plain, key.ibits );
                const natural C = key.compute( P );
                C.store( coded, key.obits );
            }
            
            //------------------------------------------------------------------
            //! pad coded to 8 bits, with noise
            //------------------------------------------------------------------
            while( coded.size() & 7 ) coded.push_back( bits() );
            
            
            //------------------------------------------------------------------
            //! pack to bytes
            //------------------------------------------------------------------
            string ans( coded.size() >> 3, as_capacity );
            while( coded.size() > 0 )
            {
                assert( coded.size() >= 8 );
                ans.append( coded.pop_full<uint8_t>() );
            }
            reset();
            return ans;
        }
        
        string rsa_auth:: signature( const void *data, size_t size,  const rsa_key &key, hashing::function &h )
        {
            assert(!(NULL==data&&size>0));
            
            memory::buffer_of<uint8_t,memory::global> hkey(h.length);
            assert( hkey.length() >= h.length );
            
            h.set();
            h.run(data,size);
            h.get( hkey.rw(), h.length);
            
            return encrypt( hkey.ro(), h.length, key ); 
        }
        
        string rsa_auth:: signature( const memory::ro_buffer &buff, const rsa_key &key, hashing::function &h )
        {
            return signature(buff.ro(), buff.length(), key, h);
        }
        
        
        string rsa_auth:: decrypt( const memory::ro_buffer &buff, const rsa_key &key )
        {
            return decrypt( buff.ro(), buff.length(), key );
        }
        
        string rsa_auth:: decrypt( const void *data, size_t size, const rsa_key &key )
        {
            assert( !(NULL==data&&size>0));
            
            reset();
            
            //------------------------------------------------------------------
            // store coded bits
            //------------------------------------------------------------------
            {
                const uint8_t *p = (const uint8_t *)data;
                for(size_t i=0; i < size; ++i ) 
                    coded.push_full<uint8_t>( p[i] );
            }
            
            //------------------------------------------------------------------
            // build plain bits
            //------------------------------------------------------------------
            const size_t items = coded.size() / key.obits;
            for( size_t i=0; i < items; ++i )
            {
                const natural C = natural::query( coded, key.obits );
                const natural P = key.compute(C);
                P.store( plain, key.ibits );
            }
            
            //------------------------------------------------------------------
            // extract data
            //------------------------------------------------------------------
            const size_t info_bits = sizeof(uint16_t) + header;
            if( plain.size() <  info_bits )
                throw exception("rsa_auth.decrypt(only %u bits)", unsigned( plain.size() ) );
            
            const size_t num = plain.pop_full<uint16_t>();
            plain.skip(header);
            if( plain.size() < (num << 3 ) )
                throw exception("rsa_auth.decrypt(missing data)");
            
            
            string ans(num,as_capacity);
            for( size_t i=0; i < num; ++i )
                ans.append( char(plain.pop_full<uint8_t>()) );
            
            reset();
            
            return ans;
        }
     
        string rsa_auth:: hash_string( const void *data, size_t size, hashing::function &h )
        {
            memory::buffer_of<uint8_t,memory::global> hkey(h.length);
            assert( hkey.length() >= h.length );
            
            h.set();
            h.run(data,size);
            h.get( hkey.rw(), h.length);
            
            string ans( h.length, as_capacity );
            for( size_t i=0; i < h.length; ++i )
                ans.append( char( hkey[i] ) );
            return ans;
        }

        string rsa_auth:: hash_string( const memory::ro_buffer &buff, hashing::function &h )
        {
            return hash_string( buff.ro(), buff.length(), h );
        }
        
        
       
    }
    
}
