#include "yocto/utest/run.hpp"
#include "yocto/mpa/rsa/key.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace mpa;

static const uint8_t rsa_keys[] =
{
#include "rsa-keys.cxx"
};

YOCTO_UNIT_TEST_IMPL(rsa_auth)
{
    vector<RSA::PrivateKey> Keys;
    {
        ios::imstream fp(rsa_keys,sizeof(rsa_keys));
        char C;
        while(fp.query(C))
        {
            fp.store(C);
            const RSA::PrivateKey k = RSA::PrivateKey::load_prv(fp);
            Keys.push_back(k);
        }
    }
    std::cerr << "Loaded #" << Keys.size() << " keys" << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
