#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(prime)
{
    mpn n;
    for(size_t i=0;i<100;++i,++n)
    {
        if( n.is_prime_() )
            std::cerr << n << std::endl;
    }
    
    n.ldz();
    for(size_t i=0;i<=20;++i,++n)
    {
        const mpn p = n.next_prime_();
        std::cerr << n << " => " << p << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(rsa)
{
    mpn p = _rand.full<uint32_t>();
    p     = p.next_prime_();
    std::cerr << "p=" << p << std::endl;
    mpn q = _rand.full<uint32_t>();
    q     = q.next_prime_();
    std::cerr << "q=" << q << std::endl;
    const mpn n = p*q;
    mpn p1 = p; --p1;
    mpn q1 = q; --q1;
    mpn phi = p1*q1;
    std::cerr << "phi=" << phi << std::endl;
    
    
    mpn e = _rand.full<uint16_t>();
    while( !mpn::are_coprime(e, phi) ) ++e;
    std::cerr << "e=" << e << std::endl;
    
    mpn d = mpn::mod_inv(e, phi);
    std::cerr << "d=" << d << std::endl;
    const size_t mbits = n.bits() -1;
    //std::cerr << "#n = " << n.bits() << std::endl;
    
    const mpn dp = d % p1;
    const mpn dq = d % q1;
    const mpn iq = mpn::mod_inv(q, p);
    
    for(size_t i=0; i <10; ++i )
    {
        const mpn M = mpn::rand(mbits);
        const mpn C = mpn::mod_exp(M, e, n);
        const mpn P = mpn::mod_exp(C,d,n);
        
        
        mpn       M1 = mpn::mod_exp(C, dp, p);
        const mpn M2 = mpn::mod_exp(C, dq, q);
        while(M1<M2) M1 += p;
        M1 -= M2;
        M1 *= iq;
        const mpn h = M1 % p;
        const mpn Q = M2 + h * q;
        
        std::cerr << M << " => " << C << " => " << P  << " / " << Q << std::endl;
        
        if( P != M) throw exception("rsa failure");
        if( Q != M) throw exception("crt failure");
    }
    
    
    }
    YOCTO_UNIT_TEST_DONE()
