#ifndef YOCTO_CRYPTO_IOS_CIPHER_INCLUDED
#define YOCTO_CRYPTO_IOS_CIPHER_INCLUDED 1

#include "yocto/hashing/sha1.hpp"
#include "yocto/hashing/hash-mac.hpp"
#include "yocto/crypto/bc/operating-block-cipher.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/functor.hpp"

#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"


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
        
        
        typedef hashing::sha1           ios_hash;
        typedef hashing::hmac<ios_hash> ios_hmac;
        
        class ios_cipher
        {
        public:
            typedef functor<bool,TL1(size_t)> callback;
            typedef uint64_t                  code_t;
            
            virtual ~ios_cipher() throw();
            
            explicit ios_cipher(ios_bits                 bits,
                                block_cipher::action     mode,
                                const string            &name,
                                const memory::ro_buffer &user_key );
            
            
            virtual bool process( ios::ostream &target, ios::istream &source, callback *cb) = 0;
            
        protected:
            ios_hash                         H;
            ios_hmac                         S;
        public:
            const size_t                     block_size; //!< AES: 16
            
        protected:
            size_t                           count;
            size_t                           total;
            uint8_t                         *obuf;
            uint8_t                         *ibuf;
            auto_ptr<block_cipher>           block_encrypter;
            auto_ptr<block_cipher>           block_decrypter;
            auto_ptr<operating_block_cipher> operating;
            const digest                     instr;
            size_t                           bmem;
                        
            //! emit an encoded block of count bytes, set count to 0, update total
            void emit( ios::ostream &target);
            
            //! reschedule cipher, count=0, total=0
            void init() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ios_cipher);
        };
    }
}



#endif

