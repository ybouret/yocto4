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
            
            init();  // prepare cipher
            S.set(); // prepare hmac
            
            //==================================================================
            // main processing
            //==================================================================
            for(;;)
            {
                assert(count<block_size);
                char &C = *(char *)&ibuf[count];
                if(source.query(C))
                {
                    if(++count>=block_size)
                    {
                        //-- update signature
                        S.run(ibuf,block_size);
                        
                        //-- encrypt
                        operating->crypt(obuf, ibuf);
                        
                        //-- emit
                        emit(target);
                        assert(0==count);
                    }
                }
                else
                    break; // EOF
            }
            
            //==================================================================
            // flushing
            //==================================================================
            if(count>0)
            {
                S.run(ibuf,block_size);
                operating->crypt_flush(obuf, ibuf, count);
                emit(target);
            }
            
            //==================================================================
            // signing
            //==================================================================
            if(false)
            {
                const code_t code = S.key<code_t>();
                target.emit(code);
            }
            return true;
        }
        
    }
}
