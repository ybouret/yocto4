#include "yocto/utest/run.hpp"
#include "yocto/mpa/rsa/codec.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
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
    
    string line;
    ios::icstream inp( ios::cstdin );
    RSA::encoder  enc(Keys[1]);
    while( line.clear(), inp.read_line(line) >= 0 )
    {
        for(size_t iter=1;iter<=4;++iter)
        {
            const string s_enc = enc.to_string(line);
            s_enc.output_visible(std::cerr<<"Enc=") << std::endl;
        }
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
