#include "yocto/utest/run.hpp"
#include "yocto/crypto/ios/encrypter.hpp"
#include "yocto/crypto/ios/decrypter.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ios)
{
    const string key = "toto";
    crypto::ios_encrypter e1( crypto::ios128, "ecb", key);
    crypto::ios_decrypter d1( crypto::ios128, "ecb", key);

    crypto::ios_encrypter e2( crypto::ios128, "cbc", key);
    crypto::ios_decrypter d2( crypto::ios128, "cbc", key);

}
YOCTO_UNIT_TEST_DONE()
