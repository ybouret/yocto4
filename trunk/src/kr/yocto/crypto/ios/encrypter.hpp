#ifndef YOCTO_CRYPTO_IOS_ENCRYPTER_INCLUDED
#define YOCTO_CRYPTO_IOS_ENCRYPTER_INCLUDED

#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/crypto/ios/cipher.hpp"

namespace yocto
{
    namespace crypto
    {
        class ios_encrypter : public ios_cipher
        {
        public:
            explicit ios_encrypter(ios_bits                 bits,
                                   const string            &name,
                                   const memory::ro_buffer &user_key );
            virtual ~ios_encrypter() throw();
            
            virtual bool process(ios::ostream &target, ios::istream &source, callback *cb);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ios_encrypter);
            
        };
    }
    
}

#endif
