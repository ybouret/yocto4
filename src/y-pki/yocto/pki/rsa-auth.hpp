#ifndef YOCTO_PKI_RSA_AUTH_INCLUDED
#define YOCTO_PKI_RSA_AUTH_INCLUDED 1


#include "yocto/pki/rsa-keys.hpp"
#include "yocto/hashing/function.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/memory/buffer.hpp"

namespace yocto
{
    namespace pki
    {
        
        class rsa_auth 
        {
        public:
            explicit rsa_auth() throw();
            explicit rsa_auth( size_t nbits ); //!< reserve nbits for plain/coded
            virtual ~rsa_auth() throw();
            static const size_t header = 32; //!< extraneous random bits

            
            string encrypt( const void *data, size_t size, const rsa_key &key );
            string encrypt( const memory::ro_buffer &buff, const rsa_key &key );
            
            string signature( const void *data, size_t size, const rsa_key &key, hashing::function &h );
            string signature( const memory::ro_buffer &buff, const rsa_key &key, hashing::function &h );
            
            string decrypt( const void *data, size_t size, const rsa_key &key );
            string decrypt( const memory::ro_buffer &buff, const rsa_key &key );
            
            static string hash_string( const void *data, size_t size, hashing::function &h );
            static string hash_string( const memory::ro_buffer &buff, hashing::function &h );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_auth);
            ios::bitio       plain;
            ios::bitio       coded;
            void reset() throw();
        };
        
    }
    
}

#endif
