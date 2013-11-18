#include "yocto/crypto/ios/decrypter.hpp"
#include "yocto/exception.hpp"

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
                        //-- decrypt
                        operating->crypt(obuf, ibuf);
                        
                        //-- update signature
                        S.run(obuf,block_size);
                        
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
                operating->crypt_flush(obuf, ibuf, count);
                S.run(obuf,block_size);
                emit(target);
            }
            
            //==================================================================
            // signature check
            //==================================================================
            if(false)
            {
                const code_t tag  = source.read<code_t>();
                const code_t code = S.key<code_t>();
                if(tag!=code)
                    throw exception("corrupted data signature");
            }
            return true;
        }
    }
}
