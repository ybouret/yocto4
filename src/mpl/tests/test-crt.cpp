#include "yocto/utest/run.hpp"
using namespace yocto;

#if defined(Y_TEST_MPA)
#include "yocto/mpa/natural.hpp"
using namespace mpa;
#define OUTNAME "mpa.bin"
#else
#include "yocto/mpl/natural.hpp"
using namespace mpl;
#define OUTNAME "mpl.bin"
#endif

#include "yocto/string/conv.hpp"

static inline void perform_CRT(const natural &M,
                               const natural &modulus,
                               const natural &publicExponent,
                               const natural &privateExponent)
{
    const natural C = mpn::mod_exp(M,publicExponent,modulus);
    const natural P = mpn::mod_exp(C,privateExponent,modulus);
    std::cerr << M << "->" << C << "->" << P << std::endl;
    if(P!=M)
        throw exception("RSA failure");
}

YOCTO_UNIT_TEST_IMPL(crt)
{

    if(argc!=4)
    {
        throw exception("usage: %s p q e", argv[0]);
    }

    const mpn p = strconv::to<int>(argv[1],"p");
    const mpn q = strconv::to<int>(argv[2],"q");
    const mpn e = strconv::to<int>(argv[3],"e");


    mpn prime1 = mpn::__next_prime(p);
    mpn prime2 = mpn::__next_prime(q);
    if(prime1<prime2)
        prime1.xch(prime2);

    const natural modulus  = prime1 * prime2;
    const natural p1m = prime1-1;
    const natural p2m = prime2-1;
    const natural phi = p1m * p2m;
    natural publicExponent = e;
    while( ! mpn::are_coprime(phi,publicExponent) ) ++publicExponent;
    const natural privateExponent = mpn::mod_inv(publicExponent,phi);
    const natural exponent1       = privateExponent % p1m;
    const natural exponent2       = privateExponent % p2m;
    const natural coefficient     = mpn::mod_inv(prime2, prime1);

    std::cerr << "prime1          = " << prime1          << std::endl;
    std::cerr << "prime2          = " << prime2          << std::endl;
    std::cerr << "modulus         = " << modulus         << std::endl;
    std::cerr << "publicExponent  = " << publicExponent  << std::endl;
    std::cerr << "privateExponent = " << privateExponent << std::endl;
    std::cerr << "exponent1       = " << exponent1       << std::endl;
    std::cerr << "exponent2       = " << exponent2       << std::endl;
    std::cerr << "coefficient     = " << coefficient     << std::endl;

    const size_t ibits = modulus.bits()-2;
    for(size_t i=0;i<=ibits;++i)
    {
        const natural M = mpn::exp2(i);
        perform_CRT(M,modulus, publicExponent, privateExponent);
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
