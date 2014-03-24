#include "yocto/utest/run.hpp"
#include "yocto/mpa/rsa/keys.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace mpa;

YOCTO_UNIT_TEST_IMPL(rsa_keys)
{
    for(size_t i=0;i<8;++i)
    {
        const mpn prime1 = mpn::rand(20).next_prime_();
        std::cerr << "prime1=" << prime1 << std::endl;
        
        const mpn prime2 = mpn::rand(20).next_prime_();
        std::cerr << "prime2=" << prime2 << std::endl;
        
        const mpn ee = mpn::rand(17);
        
        RSA::PublicKey pub = RSA::PublicKey::GenerateFrom(prime1, prime2, ee);
        std::cerr << "pub.modulus         = " << pub.modulus << std::endl;
        std::cerr << "pub.publicExponent  = " << pub.publicExponent << std::endl;
        
        RSA::PrivateKey prv = RSA::PrivateKey::GenerateFrom(prime1, prime2, ee);
        std::cerr << "prv.modulus         = " << prv.modulus         << std::endl;
        std::cerr << "prv.publicExponent  = " << prv.publicExponent  << std::endl;
        std::cerr << "prv.privateExponent = " << prv.privateExponent << std::endl;
        std::cerr << "prv.prime1          = " << prv.prime1          << std::endl;
        std::cerr << "prv.prime2          = " << prv.prime2          << std::endl;
        std::cerr << "prv.exponent1       = " << prv.exponent1       << std::endl;
        std::cerr << "prv.exponent2       = " << prv.exponent2       << std::endl;
        std::cerr << "prv.coefficient     = " << prv.coefficient     << std::endl;
        
        for(size_t j=0;j<8;++j)
        {
            const mpn M = mpn::rand( pub.ibits );
            const mpn C = pub.encode(M);
            const mpn P = prv.decode_(C);
            const mpn Q = prv.decode(C);
            if(P!=M)
                throw exception("RSA Failure");
            if(P!=Q)
                throw exception("CRT Failure");
        }
        
        for(size_t j=0;j<8;++j)
        {
            const mpn M = mpn::rand( pub.ibits );
            const mpn C = prv.encode(M);
            const mpn P = pub.decode(C);
            if(P!=M)
                throw exception("AUTH Failure");
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/ios/ocstream.hpp"
YOCTO_UNIT_TEST_IMPL(rsa_perf)
{
    const size_t N    = 256;
    const size_t ITER = 2;
    const size_t COUNT = ITER * N;
    vector<mpn> C(N,as_capacity);
    wtime chrono;
    
    ios::ocstream::overwrite("rsa.dat");
    for(size_t nbits=20;nbits<=26;++nbits)
    {
        std::cerr << "<" << nbits << ">" << std::endl;
        const mpn prime1 = mpn::rand(nbits).next_prime_();
        const mpn prime2 = mpn::rand(nbits).next_prime_();
        const mpn ee     = mpn::rand(17);
        RSA::PrivateKey prv = RSA::PrivateKey::GenerateFrom(prime1, prime2, ee);
        
        std::cerr << "encoding..." << std::endl;
        C.free();
        for(size_t i=N;i>0;--i)
        {
            const mpn m = mpn::rand(prv.ibits);
            const mpn c = prv.PublicKey::encode(m);
            C.push_back( c );
        }
        
        std::cerr << "decoding: RAW" << std::endl;
        chrono.start();
        for(size_t iter=0;iter<ITER;++iter)
        {
            for(size_t i=N;i>0;--i)
            {
                prv.decode_(C[i]);
            }
        }
        const double raw = chrono.query();
        std::cerr << "raw=" << raw << std::endl;
        chrono.start();
        for(size_t iter=0;iter<ITER;++iter)
        {
            for(size_t i=N;i>0;--i)
            {
                prv.decode(C[i]);
            }
        }
        const double crt = chrono.query();
        std::cerr << "crt=" << crt << std::endl;
        ios::ocstream fp("rsa.dat",true);
        fp("%u %g %g %g\n", unsigned(nbits), COUNT/raw, COUNT/crt, raw/crt );
        }
        
        }
        YOCTO_UNIT_TEST_DONE()
        
        
