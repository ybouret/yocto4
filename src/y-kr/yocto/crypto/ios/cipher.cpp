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
            memory::kind<memory::global>::release_as<uint8_t>(obuf, bmem);
        }
        
        ios_cipher:: ios_cipher(ios_bits                 bits,
                                block_cipher::action     mode,
                                const string            &name,
                                const memory::ro_buffer &user_key ):
        H(),
        S(user_key),
        block_size(16),
        count(0),
        total(0),
        obuf(0),
        ibuf(0),
        block_encrypter(),
        block_decrypter(),
        operating(),
        instr( digest::checksum(S,user_key) ),
        bmem(0)
        {
            //==================================================================
            // get the factory
            //==================================================================
            static const block_cipher_factory &F = block_cipher_factory::instance();
            
            //==================================================================
            // build up the key
            //==================================================================
            const digest Key = digest::checksum(H,user_key);
            //std::cerr << "Key=" << Key   << std::endl;
            //std::cerr << "IV =" << instr << std::endl;
            
            //==================================================================
            // create the block_cipher pair
            //==================================================================
            switch(bits)
            {
                case ios128:
                    block_encrypter.reset( new aes128::encrypter(Key) );
                    block_decrypter.reset( new aes128::decrypter(Key) );
                    break;
                    
                case ios192:
                    block_encrypter.reset( new aes192::encrypter(Key) );
                    block_decrypter.reset( new aes192::decrypter(Key) );
                    break;
                    
                case ios256:
                    block_encrypter.reset( new aes256::encrypter(Key) );
                    block_decrypter.reset( new aes256::decrypter(Key) );
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
            //std::cerr << "using " << id << std::endl;
            operating.reset( F.create(id, *block_encrypter, *block_decrypter, instr) );
            
            bmem   = block_size*2;
            obuf   = memory::kind<memory::global>::acquire_as<uint8_t>(bmem);
            ibuf   = &obuf[block_size];
        }
        
        
        void ios_cipher:: init() throw()
        {
            operating->schedule(instr);
            count = 0;
            total = 0;
        }
        
        
        void ios_cipher:: emit( ios::ostream &target)
        {
            assert(count>0);
            target.save(obuf,count);
            total += count;
            count  = 0;
        }
        
        
    }
}
