#include "yocto/crypto/ios/cipher.hpp"
#include "yocto/crypto/bc/block-cipher-factory.hpp"
#include "yocto/crypto/bc/aes.hpp"
#include <iostream>

namespace yocto
{
    namespace crypto
    {
        ios_cipher:: ~ios_cipher() throw()
        {
            memory::kind<memory::global>::release(buffer, buflen);
        }
        
        ios_cipher:: ios_cipher(ios_bits                 bits,
                                block_cipher::action     mode,
                                const string            &name,
                                const memory::ro_buffer &user_key ):
        H(),
        S(user_key),
        block_size(0),
        block_encrypter(),
        block_decrypter(),
        instr( digest::checksum(S,user_key) )
        {
            //==================================================================
            // get the factory
            //==================================================================
            static const block_cipher_factory &F = block_cipher_factory::instance();
            
            //==================================================================
            // build up the key
            //==================================================================
            const digest Key = digest::checksum(H,user_key);
            std::cerr << "Key=" << Key   << std::endl;
            std::cerr << "IV =" << instr << std::endl;
            
            //==================================================================
            // create the block_cipher pair
            //==================================================================
            switch(bits)
            {
                case ios128:
                    block_encrypter.reset( new aes128::encrypter(Key) );
                    block_decrypter.reset( new aes128::decrypter(Key) );
                    (size_t &) block_size = 128/8;
                    break;
                    
                case ios192:
                    block_encrypter.reset( new aes192::encrypter(Key) );
                    block_decrypter.reset( new aes192::decrypter(Key) );
                    (size_t &) block_size = 192/8;
                    break;
                    
                case ios256:
                    block_encrypter.reset( new aes256::encrypter(Key) );
                    block_decrypter.reset( new aes256::decrypter(Key) );
                    (size_t &) block_size = 256/8;
                    break;
            }
            
            //==================================================================
            // get the name of the operating block cipher
            //==================================================================
            string id;
            switch(mode)
            {
                case block_cipher::encrypting: id = "encrypt"; break;
                case block_cipher::decrypting: id = "decrypt"; break;
            }
            id += '_';
            id += name;
            std::cerr << "using " << id << std::endl;
            operating.reset( F.create(id, *block_encrypter, *block_decrypter, instr) );
            
            buflen = block_size;
            buffer = memory::kind<memory::global>::acquire(buflen);
            
        }
        
        void ios_cipher:: restart() throw()
        {
            operating->schedule(instr);
        }

    }
}