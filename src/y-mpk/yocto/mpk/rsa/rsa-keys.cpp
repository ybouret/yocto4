#include "yocto/mpk/rsa/keys.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto
{
	
	namespace mpk
	{
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
#define __LDZ(VAR) ((natural &)VAR).ldz()
		
		rsa_key:: ~rsa_key() throw()
		{
			__LDZ(modulus);
		}
		
		static inline size_t __maxbits_for( const natural &m )
		{
			natural n = m;
			--n;
			const size_t ans = n.bits();
			if( ans <= 2 ) 
				throw libc::exception( EINVAL, "rsa_key(invalid modulus)" );
			return ans;
		}
		
		rsa_key:: rsa_key( const natural &rsa_modulus ) :
		modulus( rsa_modulus ),
		maxbits( __maxbits_for(modulus) )
		{
			//-- check
		}
		
		rsa_key:: rsa_key( const rsa_key &k ) :
		modulus( k.modulus ),
		maxbits( k.maxbits )
		{
		}
		
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
		rsa_public_key:: rsa_public_key( const natural &rsa_modulus, const natural &rsa_publicExponent ):
		rsa_key( rsa_modulus ),
		publicExponent( rsa_publicExponent )
		{
			if( ! publicExponent.is_positive() ) 
				throw libc::exception( EINVAL, "rsa_public_key(invalid publicExponent)");
		}
		
		rsa_public_key:: ~rsa_public_key() throw()
		{
			__LDZ(publicExponent);
		}
		
		rsa_public_key:: rsa_public_key( const rsa_public_key &k ) :
		rsa_key( k ),
		publicExponent( k.publicExponent )
		{
		}
		
		
		natural rsa_public_key::compute( const natural &C ) const
		{
			return natural::mod_exp( C, publicExponent, modulus );
		}
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
		rsa_private_key:: rsa_private_key(const natural  &rsa_modulus,
										  const natural  &rsa_publicExponent,
										  const natural  &rsa_privateExponent,
										  const natural  &rsa_prime1,
										  const natural  &rsa_prime2,
										  const natural  &rsa_exponent1,
										  const natural  &rsa_exponent2,
										  const natural  &rsa_coefficient) :
		rsa_public_key( rsa_modulus, rsa_publicExponent ),
		privateExponent( rsa_privateExponent ),
		prime1( rsa_prime1 ),
		prime2( rsa_prime2 ),
		exponent1( rsa_exponent1 ),
		exponent2( rsa_exponent2 ),
		coefficient( rsa_coefficient )
		{
			static const uint8_t One = 1;
#define __CHECK_PRV(EXPR) if( !(EXPR) ) throw libc::exception( EINVAL, "rsa_private_key( INVALID %s )", #EXPR )
			__CHECK_PRV( privateExponent.is_positive() );
			__CHECK_PRV( prime1>=prime2 );
			__CHECK_PRV( exponent1   == privateExponent % (prime1 - One ) );
			__CHECK_PRV( exponent2   == privateExponent % (prime2 - One ) );
			__CHECK_PRV( coefficient == natural::mod_inv( prime2, prime1 ) ); 
		}
		
		rsa_private_key:: ~rsa_private_key() throw()
		{
			__LDZ(privateExponent);
			__LDZ(prime1);
			__LDZ(prime2);
			__LDZ(exponent1);
			__LDZ(exponent2);
			__LDZ(coefficient);
		}
		
		rsa_private_key:: rsa_private_key( const rsa_private_key &k ) :
		rsa_public_key( k ),
		privateExponent( k.privateExponent ),
		prime1( k.prime1 ),
		prime2( k.prime2 ),
		exponent1( k.exponent1 ),
		exponent2( k.exponent2 ),
		coefficient( k.coefficient )
		{
		}
		
		natural rsa_private_key::compute( const natural &C ) const
		{
			natural        M1 = natural::mod_exp( C, exponent1, prime1 );
			const natural  M2 = natural::mod_exp( C, exponent2, prime2 );
			while( M1 < M2 )
				M1 += prime1;
			return  M2 + ( ( (M1-M2) * coefficient ) % prime1 ) * prime2;
		}
		
	}
	
}