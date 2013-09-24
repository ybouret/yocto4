#include "yocto/crypto/ios/cipher.hpp"
#include <iostream>

//#include "yocto/crypto/bc/block-cipher-ecb.hpp"

namespace yocto
{
    namespace crypto
    {
        ios_cipher:: ~ios_cipher() throw()
        {
        }
        
        ios_cipher:: ios_cipher(ios_bits bits,
                                ios_mode mode,
                                const memory::ro_buffer &user_key ):
        H(),
        S(user_key),
        block_size(0),
        block_encrypter(),
        block_decrypter(),
        instr( digest::checksum(S,user_key) )
        {
            
            const digest Key = digest::checksum(H,user_key);
            std::cerr << "Key=" << Key   << std::endl;
            std::cerr << "IV =" << instr << std::endl;
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
            
#if 0
            switch(mode)
            {
                case iosECB:
                    encrypter.reset( new block_cipher_ecb::encrypter( *block_encrypter, instr ) );
                    decrypter.reset( new block_cipher_ecb::decrypter( *block_decrypter, *block_encrypter, instr ) );
                    break;
                    
            }
#endif
        }
        
        
    }
}