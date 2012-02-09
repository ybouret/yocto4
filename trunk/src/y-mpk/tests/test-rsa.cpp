#if 1
#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/wtime.hpp"
#include "yocto/random/isaac.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/primes.hpp"

using namespace yocto;

#define N 32

YOCTO_UNIT_TEST_IMPL(rsa0)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );
	
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
	
	
}
YOCTO_UNIT_TEST_DONE()
#endif

#include "yocto/mpk/rsa/keys.hpp"

YOCTO_UNIT_TEST_IMPL(rsa1)
{
	Random::ISAAC_FAST::BitsGenerator gen( Random::ISAAC_INIT_RAND );

	mpn prime1 = gen.rand<uint64_t>(34);
	mpn prime2 = gen.rand<uint64_t>(34);
	std::cerr << "Generating prime1" << std::endl;
	prime1 = prime1.next_prime_();
	
	std::cerr << "Generating prime2" << std::endl;
	prime2 = prime2.next_prime_();
	if( prime1 < prime2 )
		prime1.xch(prime2);
	
	std::cerr << "prime1=" << prime1 << std::endl;
	std::cerr << "prime2=" << prime2 << std::endl;
	
	const mpn modulus  = prime1 * prime2;
	mpn       p1m      = prime1; --p1m;
	mpn       p2m      = prime2; --p2m;
	const mpn z        = p1m * p2m;
	mpn publicExponent = gen.full<uint32_t>();
	if( !publicExponent.is_odd() )
		++publicExponent;
	while( !mpn::are_coprime(publicExponent,z) )
		publicExponent.inc(2);
	
	std::cerr << "publicExponent  = " << publicExponent << std::endl;
	
	const mpk::rsa_public_key pub( modulus, publicExponent );
	
	const mpn privateExponent = mpn::mod_inv(publicExponent,z);
	std::cerr << "privateExponent = " << privateExponent << std::endl;
	const mpn exponent1       = privateExponent % p1m;
	const mpn exponent2       = privateExponent % p2m;
	const mpn coefficient     = mpn::mod_inv( prime2, prime1 );
	std::cerr << "exponent1       = " << exponent1   << std::endl;
	std::cerr << "exponent2       = " << exponent2   << std::endl;
	std::cerr << "coefficient     = " << coefficient << std::endl;
	
	const mpk::rsa_private_key prv( modulus, publicExponent, privateExponent, prime1, prime2, exponent1, exponent2, coefficient );
	
	for( int i=0; i < N; ++i )
	{
		const mpn P = mpn( gen.full<uint64_t>() ) % modulus;
		std::cerr << "P=" << P << " "; std::cerr.flush();
		const mpn C = pub.compute( P );
		std::cerr << "C=" << C << " "; std::cerr.flush();
		const mpn D = prv.compute( C );
		std::cerr << "D=" << D;
			
		std::cerr << std::endl;
		if( P != D )
			throw exception("RSA error...");

	}
	
}
YOCTO_UNIT_TEST_DONE()
