#include "yocto/pki/rsa-keys.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/ios/istream.hpp"

#include <cerrno>

namespace yocto
{
	
	namespace pki
	{
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
#define __LDZ(VAR) ((natural &)VAR).ldZ()
		
		rsa_key:: ~rsa_key() throw()
		{
			__LDZ(modulus);
		}
		
		static inline size_t __obits_for( const natural &m )
		{
			natural n = m;
			--n;
			const size_t ans = n.bits();
			if( ans <= 3 )
				throw libc::exception( EINVAL, "rsa_key(invalid modulus)" );
			return ans;
		}
		
		rsa_key:: rsa_key( const natural &rsa_modulus ) :
		modulus( rsa_modulus ),
		obits( __obits_for(modulus) ),
        ibits( obits-1 )
		{
		}
		
		rsa_key:: rsa_key( const rsa_key &k ) :
		modulus( k.modulus ),
		obits( k.obits ),
        ibits( k.ibits )
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
		
        void rsa_public_key:: save_pub( ios::ostream &fp ) const
        {
            fp.emit( PUB );
            modulus.save( fp );
            publicExponent.save( fp );
        }
        
        rsa_public_key *rsa_public_key::load_pub( ios::istream &fp )
        {
            const uint32_t type_of_key = fp.read<uint32_t>();
            switch( type_of_key )
            {
                case PUB:
                {
                    const natural _modulus        = natural::load(fp);
                    const natural _publicExponent = natural::load(fp);
                    return new rsa_public_key( _modulus, _publicExponent );
                }
                    
                case PRV:
                {
                    const natural _modulus         = natural::load(fp);
                    const natural _publicExponent  = natural::load(fp);
                    const natural _privateExponent = natural::load(fp);
                    const natural _prime1          = natural::load(fp);
                    const natural _prime2          = natural::load(fp);
                    const natural _exponent1       = natural::load(fp);
                    const natural _exponent2       = natural::load(fp);
                    const natural _coefficient     = natural::load(fp);
                    __LDZ(_privateExponent);
                    __LDZ(_prime1);
                    __LDZ(_prime2);
                    __LDZ(_exponent1);
                    __LDZ(_exponent2);
                    __LDZ(_coefficient);
                    return new rsa_public_key( _modulus, _publicExponent );
                }
                    break;
                default: break;
            }
            throw exception("rsa_public_key::load_pub( invalid key type )");
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
        
        void rsa_private_key:: save_prv( ios::ostream &fp ) const
        {
            fp.emit( PRV );
            modulus.save( fp );
            publicExponent.save( fp );
            privateExponent.save(fp);
            prime1.save(fp);
            prime2.save(fp);
            exponent1.save(fp);
            exponent2.save(fp);
            coefficient.save(fp);
        }
        
        rsa_private_key *rsa_private_key::load_prv( ios::istream &fp )
        {
            if( PRV != fp.read<uint32_t>() )
            {
                throw exception("rsa_private_key::load_prv( invalid key type )");
            }
            
            const natural _modulus         = natural::load(fp);
            const natural _publicExponent  = natural::load(fp);
            const natural _privateExponent = natural::load(fp);
            const natural _prime1          = natural::load(fp);
            const natural _prime2          = natural::load(fp);
            const natural _exponent1       = natural::load(fp);
            const natural _exponent2       = natural::load(fp);
            const natural _coefficient     = natural::load(fp);
            try
            {
                rsa_private_key *prv = new rsa_private_key(_modulus,
                                                           _publicExponent,
                                                           _privateExponent,
                                                           _prime1,
                                                           _prime2,
                                                           _exponent1,
                                                           _exponent2,
                                                           _coefficient);
                __LDZ(_privateExponent);
                __LDZ(_prime1);
                __LDZ(_prime2);
                __LDZ(_exponent1);
                __LDZ(_exponent2);
                __LDZ(_coefficient);
                return prv;
            }
            catch(...)
            {
                __LDZ(_privateExponent);
                __LDZ(_prime1);
                __LDZ(_prime2);
                __LDZ(_exponent1);
                __LDZ(_exponent2);
                __LDZ(_coefficient);
                throw;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
		
	}
	
}

