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
            size_t count = 0;
            size_t done  = 0;
            S.set();
            for(;;)
            {
                source.get(ibuf, block_size, done);
                if(done<=0)
                {
                    break;
                }
                S.run(ibuf,done);
                if(done>=block_size)
                {
                    assert(done==block_size);
                    std::cerr << "full" << std::endl;
                    operating->crypt(obuf, ibuf);
                }
                else
                {
                    std::cerr << "part" << std::endl;
                    operating->crypt_flush(obuf, ibuf, done);
                }
                target.save(obuf, done);
                count += done;
            }
            std::cerr << "processed #" << count << std::endl;
            return true;

        }
    }
}
