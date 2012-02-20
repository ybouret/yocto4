#ifndef YOCTO_PKI_RSA_AUTH_INCLUDED
#define YOCTO_PKI_RSA_AUTH_INCLUDED 1


#include "yocto/pki/rsa-keys.hpp"
#include "yocto/hashing/function.hpp"
#include "yocto/ios/bitio.hpp"

namespace yocto
{
    namespace pki
    {
        
        class rsa_auth 
        {
        public:
            explicit rsa_auth( const rsa_private_key &prv );
            virtual ~rsa_auth() throw();

            string encrypt( const void *data, size_t size, hashing::function &h);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_auth);
            rsa_key::pointer prv_k;
            rsa_key::pointer pub_k;
            ios::bitio       plain;
            ios::bitio       coded;
        };
        
    }
    
}

#endif
