#include "yocto/utest/run.hpp"
#include "yocto/crypto/ios/encrypter.hpp"
#include "yocto/crypto/ios/decrypter.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/osstream.hpp"

using namespace yocto;

static inline
void __encipher( ios::ostream &target, ios::istream &source, crypto::ios_cipher &enc)
{
    enc.process(target,source,0);
}

static inline
void __check( crypto::ios_cipher &enc, crypto::ios_cipher &dec )
{
    string       src = "QWERTYUIOPASDFGHJKLZXCVBNM";
    src += src;
    string       tgt;
    
    {
        ios::imstream source(src);
        ios::osstream target(tgt);
        __encipher(target, source, enc);
        tgt.output_visible(std::cerr) << std::endl;
    }
    
    string chk;
    {
        ios::imstream source(tgt);
        ios::osstream target(chk);
        __encipher(target,source,dec);
        chk.output_visible(std::cerr) << std::endl;
    }
    
}

YOCTO_UNIT_TEST_IMPL(ios)
{
    const string key = "toto";
    crypto::ios_encrypter e1( crypto::ios128, "ecb", key);
    crypto::ios_decrypter d1( crypto::ios128, "ecb", key);
    
    crypto::ios_encrypter e2( crypto::ios128, "cbc", key);
    crypto::ios_decrypter d2( crypto::ios128, "cbc", key);
    
    __check(e1, d1);
    
    __check(e2,d2);
    
    
}
YOCTO_UNIT_TEST_DONE()
