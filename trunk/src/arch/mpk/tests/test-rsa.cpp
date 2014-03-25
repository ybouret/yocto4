#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/random/isaac.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/primes.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

#define N 32

YOCTO_UNIT_TEST_IMPL(rsa0)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	wtime chrono;
    
    chrono.start();
	mpn p = gen.rand<uint64_t>(34);
	mpn q = gen.rand<uint64_t>(34);
	std::cerr << "Generating p" << std::endl;
	p = p.next_prime_();
	
	std::cerr << "Generating q" << std::endl;
	q = q.next_prime_();
	if( p < q )
		p.xch(q);
	
	std::cerr<< "p=" << p << std::endl;
	std::cerr << "q=" << q << std::endl;
	
	mpn n   = p*q;
	mpn pm1 = p; --pm1;
	mpn qm1 = q; --qm1;
	mpn z   = pm1 * qm1;
	std::cerr << "n=" << n << "(#bits=" << n.bits() << ")" << std::endl;
	std::cerr << "z=" << z << std::endl;
	
	mpn e = gen.full<uint32_t>();
	if( !e.is_odd() )
		++e;
	while( !mpn::are_coprime(e,z) )
		e.inc(2);
	
	std::cerr << "e=" << e << std::endl;
	mpn d = mpn::mod_inv( e, z );
	std::cerr << "d=" << d << std::endl;
	
	const mpn d1 = d % pm1; std::cerr << "d1=" << d1 << std::endl;
	const mpn dP = mpn::mod_inv( e, pm1 ); std::cerr << "dP=" << dP << std::endl;
	
	const mpn d2 = d % qm1; std::cerr << "d2=" << d2 << std::endl;
	const mpn dQ = mpn::mod_inv( e, qm1 ); std::cerr << "dQ=" << dQ << std::endl;
	
	const mpn cf   = mpn::mod_inv( q, p ); std::cerr << "cf  =" << cf << std::endl;
	const mpn dInv = mpn::mod_inv( q, p ); std::cerr << "dInv=" << dInv << std::endl;
	
	for( int i=0; i < N; ++i )
	{
		const mpn P = mpn( gen.full<uint64_t>() ) % n;
		const mpn C = mpn::mod_exp( P, e, n );
		const mpn D = mpn::mod_exp( C, d, n );
		
		std::cerr << "raw=" << P << "->" << C << "->" << D;
		std::cerr.flush();
		mpn       M1 = mpn::mod_exp(C,d1,p);
		mpn       M2 = mpn::mod_exp(C,d2,q);
		while( M1 < M2 ) M1 += p;
		const mpn M  = M2 + ( ( (M1-M2) * cf ) % p ) * q;
		std::cerr << "/crt=" << M << std::endl;
	}
	const double ell = chrono.query();
    std::cerr << "Ellapsed=" << ell << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/code/rand.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/mpk/natural.hpp"

using namespace mpk;

YOCTO_UNIT_TEST_IMPL(bitio)
{
    ios::bitio bio;
    for( size_t iter=0; iter <= 16; ++ iter )
    {
        string s;
        for( size_t i=0; i < iter; ++i )
        {
            if( alea<float>() > 0.5f )
            {
                s += char('0'+alea_lt(10));
            }
            else
                s += char('A'+alea_lt(6));
        }
        const natural x = natural::hex( s );
        std::cerr << "x=" << x << " / bits=" << x.bits() << std::endl;
        for( size_t k=0; k <= 10; ++k )
        {
            bio.free();
            x.store(bio, x.bits() + k );
            const natural y = natural::query(bio, x.bits() + k );
            if( x != y )
                throw exception("bitio mismatch");
        }
        
    }
}
YOCTO_UNIT_TEST_DONE()


