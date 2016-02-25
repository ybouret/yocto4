#include "yocto/mpl/rsa/keys.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

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
    
}
YOCTO_UNIT_TEST_DONE()