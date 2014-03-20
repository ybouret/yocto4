#include "yocto/utest/run.hpp"
#include "yocto/mpa/rsa/key.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace mpa;

YOCTO_UNIT_TEST_IMPL(rsa_keys)
{
    for(size_t i=0;i<10;++i)
    {
        const mpn prime1 = mpn::rand(20).next_prime_();
        std::cerr << "prime1=" << prime1 << std::endl;
        
        const mpn prime2 = mpn::rand(20).next_prime_();
        std::cerr << "prime2=" << prime2 << std::endl;
        
        const mpn ee = 65537;
        
        RSA::PublicKey pub = RSA::PublicKey::GenerateFrom(prime1, prime2, ee);
        std::cerr << "pub.modulus         = " << pub.modulus << std::endl;
        std::cerr << "pub.publicExponent  = " << pub.publicExponent << std::endl;
        
        RSA::PrivateKey prv = RSA::PrivateKey::GenerateFrom(prime1, prime2, ee);
        std::cerr << "prv.modulus         = " << prv.modulus << std::endl;
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
            const mpn C = pub.encode_with_pub(M);
            const mpn P = prv.decode_with_prv_(C);
            std::cerr << M << " => " << C << " => " << P << std::endl;
            if( P != M )
                throw exception("RSA Failure");
        }
        
        
    }
    
}
YOCTO_UNIT_TEST_DONE()

