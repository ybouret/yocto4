#include "yocto/utest/run.hpp"
#include "yocto/mpa/rsa/codec.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace mpa;

static const uint8_t rsa_keys[] =
{
#include "rsa-keys.cxx"
};

YOCTO_UNIT_TEST_IMPL(rsa_auth)
{
    
    size_t idx = 0;
    if( argc > 1 )
        idx = strconv::to<size_t>(argv[1],"#key");
    
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
    
    string        line;
    ios::icstream inp( ios::cstdin );
    const RSA::PrivateKey &prv = Keys[1+(idx%Keys.size())];
    const RSA::PublicKey  &pub = prv;
    RSA::encoder  enc(pub);
    while( line.clear(), (std::cerr << ">").flush(), inp.read_line(line) >= 0 )
    {
        
        const string s_enc = enc.to_string(line);
        s_enc.output_visible(std::cerr<<"Enc=") << std::endl;
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
