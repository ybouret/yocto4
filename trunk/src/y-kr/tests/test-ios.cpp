#include "yocto/utest/run.hpp"
#include "yocto/crypto/ios/encrypter.hpp"
#include "yocto/crypto/ios/decrypter.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/osstream.hpp"

using namespace yocto;

static inline
void __encrypt( ios::ostream &target, ios::istream &source, crypto::ios_cipher &enc)
{
    enc.restart();
    enc.process(target,source,0);
}

YOCTO_UNIT_TEST_IMPL(ios)
{
    const string key = "toto";
    crypto::ios_encrypter e1( crypto::ios128, "ecb", key);
    crypto::ios_decrypter d1( crypto::ios128, "ecb", key);
    
    crypto::ios_encrypter e2( crypto::ios128, "cbc", key);
    crypto::ios_decrypter d2( crypto::ios128, "cbc", key);
    
    string       src = "QWERTYUIOPASDFGHJKLZXCVBNM";
    src += src;
    string       tgt;
    
    {
        ios::imstream source(src);
        tgt.clear();
        ios::osstream target(tgt);
        __encrypt(target, source, e1);
        tgt.output_visible(std::cerr) << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
