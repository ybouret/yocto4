#ifndef YOCTO_CRYPTO_IOS_CIPHER_INCLUDED
#define YOCTO_CRYPTO_IOS_CIPHER_INCLUDED 1

#include "yocto/hashing/sha1.hpp"
#include "yocto/hashing/hash-mac.hpp"
#include "yocto/crypto/bc/aes.hpp"
#include "yocto/crypto/bc/operating-block-cipher.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace crypto
    {
        
        enum ios_bits
        {
            ios128,
            ios192,
            ios256
        };
        
        enum ios_mode
        {
            iosECB
        };
        
        typedef hashing::sha1           ios_hash;
        typedef hashing::hmac<ios_hash> ios_hmac;
        
        class ios_cipher
        {
        public:
            
            
            virtual ~ios_cipher() throw();
            
            explicit ios_cipher(ios_bits                 bits,
                                ios_mode                 mode,
                                const memory::ro_buffer &user_key );
            
        protected:
            ios_hash                         H;
            ios_hmac                         S;
            const size_t                     block_size;
            auto_ptr<block_cipher>           block_encrypter;
            auto_ptr<block_cipher>           block_decrypter;
            const digest                     instr;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ios_cipher);
        };
    }
}



#endif

