#ifndef YOCTO_MPK_RSA_KEYS_INCLUDED
#define YOCTO_MPK_RSA_KEYS_INCLUDED 1


#include "yocto/mpk/natural.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	
	namespace pki
	{
		using mpk::natural;
        
		//! base class for RSA keys
		class rsa_key : public object
		{
		public:
            static const uint32_t PUB = YOCTO_FOURCC('P','U','B',':');
            static const uint32_t PRV = YOCTO_FOURCC('P','R','V',':');
			virtual ~rsa_key() throw();
			rsa_key( const rsa_key & );
			
			const natural modulus; //!< a valid modulus
			const size_t  obits;   //!< (modulus-1).bits
			const size_t  ibits;   //!< obits-1;
            
			virtual natural  compute( const natural & ) const = 0;            
			typedef shared_ptr<rsa_key> pointer;
            
		protected:
			explicit rsa_key( const natural &rsa_modulus );
			
		private:
			YOCTO_DISABLE_ASSIGN(rsa_key);
		};
		
		
		//! RSA Public Key
		class rsa_public_key : public rsa_key
		{
		public:
			const natural publicExponent;
			virtual natural compute( const natural & ) const;
			
			virtual ~rsa_public_key() throw();
			rsa_public_key( const rsa_public_key &);
			explicit rsa_public_key( const natural &rsa_modulus, const natural &rsa_publicExponent );
		
            void                   save_pub( ios::ostream &fp ) const;
            static rsa_public_key *load_pub( ios::istream &fp );
            
		private:
			YOCTO_DISABLE_ASSIGN(rsa_public_key);
		};
		
		//! RSA Private Key
		class rsa_private_key : public rsa_public_key
		{
		public:
			virtual ~rsa_private_key() throw();
			explicit rsa_private_key(const natural  &rsa_modulus,
									 const natural  &rsa_publicExponent,
									 const natural  &rsa_privateExponent,
									 const natural  &rsa_prime1,
									 const natural  &rsa_prime2,
									 const natural  &rsa_exponent1,
									 const natural  &rsa_exponent2,
									 const natural  &rsa_coefficient);
			
			rsa_private_key( const rsa_private_key & );
			
			virtual natural compute( const natural &x ) const;
			void                    save_prv( ios::ostream &fp ) const;
            static rsa_private_key *load_prv( ios::istream &fp );
            
            
            
		private:
			const natural privateExponent; //!< (1/publicExponent) %( (prime1-1) * (prime2-1) )
			const natural prime1;          //!< bigger  prime
			const natural prime2;          //!< smaller prime
			const natural exponent1;       //!< Chinese Remainder Theoreme exponent1   = d % (prime1-1)
			const natural exponent2;       //!< Chinese Remainder Theoreme exponent2   = d % (prime2-1)
			const natural coefficient;     //!< Chinese Remainder Theoreme coefficient = (1/prime2)  % prime1
			YOCTO_DISABLE_ASSIGN( rsa_private_key );
		};
		
		
		
		
	}
	
}

#endif
