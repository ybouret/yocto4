#include "yocto/mpl/rsa/keys.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/rt-clock.hpp"

using namespace yocto;
using namespace mpl;

YOCTO_UNIT_TEST_IMPL(rsa)
{
    mpn p = mpn::rand(6+alea_leq(25));
    mpn q = mpn::rand(6+alea_leq(25));
    p = mpn::__next_prime(p);
    q = mpn::__next_prime(q);
    mpn e = 3+alea_leq(100);
    RSA::PrivateKey prv = RSA::PrivateKey::GenerateFrom(p, q, e);
    RSA::PublicKey  pub(prv);

    std::cerr << std::hex;
    std::cerr << "prv=[" << prv.modulus << "," << prv.privateExponent << "]" << std::endl;
    std::cerr << "pub=[" << pub.modulus << "," << prv.publicExponent  << "]" << std::endl;

    rt_clock chrono;
    chrono.calibrate();
    uint64_t _rsa = 0;
    uint64_t _crt = 0;
    uint64_t mark = 0;
    
    for(size_t i=1;i<=1000;++i)
    {
        const mpn M = mpn::rand( pub.ibits );
        const mpn C = pub.encode(M);
        
        mark = chrono.ticks();
        const mpn P = prv.decode_(C);
        _rsa += chrono.ticks()-mark;
        
        mark = chrono.ticks();
        const mpn Q = prv.decode(C);
        _crt += chrono.ticks()-mark;
        if(P!=M)  throw exception("RSA Failure");
        if(P!=Q)
        {
            //std::cerr << "CRT Failure" << std::endl;
            throw exception("CRT Failure");
        }
    }
    
    const double tmx_rsa = chrono(_rsa);
    const double tmx_crt = chrono(_crt);
    std::cerr << "rsa: " << tmx_rsa << std::endl;
    std::cerr << "crt: " << tmx_crt << std::endl;
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/mpl/rsa/codec.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/string/base64.hpp"

YOCTO_UNIT_TEST_IMPL(rsaQ)
{
    mpn p = mpn::rand(6+alea_leq(30));
    mpn q = mpn::rand(6+alea_leq(30));
    p = mpn::__next_prime(p);
    q = mpn::__next_prime(q);
    mpn e = 3+alea_leq(100);
    RSA::PrivateKey prv = RSA::PrivateKey::GenerateFrom(p, q, e);
    RSA::PublicKey  pub(prv);
    std::cerr << "pub=[" << pub.modulus << "," << pub.publicExponent << "]" << std::endl;

    base64::encoder b64;
    
    string line;
    ios::icstream fp( ios::cstdin );
    RSA::encoder EncPub(pub);
    //RSA::encoder EncPrv(prv);
    //RSA::decoder DecPub(pub);
    RSA::decoder DecPrv(prv);

    std::cerr << std::hex;
    while( line.clear(), (std::cerr << "> ").flush(), fp.read_line(line) >= 0 )
    {
        const string pub_enc     = EncPub.to_string(line);
        const string pub_enc_out = b64.to_string(pub_enc);
        std::cerr << "pub_enc: " << pub_enc_out << std::endl;
        const string pub_enc_dec = DecPrv.to_string(pub_enc);
        
        /*
        const string prv_enc     = EncPrv.to_string(line);
        const string prv_enc_out = b64.to_string(prv_enc);
        std::cerr << "prv_enc: " << prv_enc_out << std::endl;

         */

    }


}
YOCTO_UNIT_TEST_DONE()