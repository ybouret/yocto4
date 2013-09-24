#include "yocto/crypto/ios/decrypter.hpp"
#include <iostream>

namespace yocto
{
    namespace crypto
    {
        ios_decrypter:: ~ios_decrypter() throw() {}
        
        ios_decrypter:: ios_decrypter(ios_bits                 bits,
                                      const string            &name,
                                      const memory::ro_buffer &user_key ) :
        ios_cipher( bits, block_cipher::decrypting, name, user_key )
        {
        }
        
        bool ios_decrypter:: process(ios::ostream &target, ios::istream &source, callback *cb)
        {
            size_t count = 0;
            size_t done  = 0;
            for(;;)
            {
                source.get(ibuf, block_size, done);
                if(done<=0)
                {
                    break;
                }
                count += done;
            }
            std::cerr << "processed #" << count << std::endl;
            return true;
        }
    }
}
