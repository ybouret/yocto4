#include "yocto/crypto/ios/encrypter.hpp"
#include <iostream>

namespace yocto
{
    namespace crypto
    {
        ios_encrypter:: ~ios_encrypter() throw() {}
        
        ios_encrypter:: ios_encrypter(ios_bits                 bits,
                                      const string            &name,
                                      const memory::ro_buffer &user_key ) :
        ios_cipher( bits, block_cipher::encrypting, name, user_key )
        {
        }
        
        
        bool ios_encrypter:: process(ios::ostream &target, ios::istream &source, callback *cb)
        {
            S.set();
            
            
            return true;

        }
    }
}
