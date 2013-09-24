#include "yocto/utest/run.hpp"
#include "yocto/crypto/ios/cipher.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ios)
{
    const string key = "toto";
    crypto::ios_cipher cph( crypto::ios128, crypto::iosECB, key);
    
}
YOCTO_UNIT_TEST_DONE()
