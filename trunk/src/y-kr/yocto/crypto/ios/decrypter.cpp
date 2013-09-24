#include "yocto/crypto/ios/decrypter.hpp"

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
            
            return true;
        }
    }
}
