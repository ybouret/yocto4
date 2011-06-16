#ifndef _YOCTO_BLOCK_CIPHER_ECB_
#define _YOCTO_BLOCK_CIPHER_ECB_ 1

#include "yocto/crypto/bc/operating-block-cipher.hpp"

namespace yocto {
	
	namespace crypto {
		
		//! Electronic Code Book
		struct block_cipher_ecb {
			
			//! \f$ C_i = E_K( P_i ) \f$
			class encrypter : public operating_block_cipher {
			public:
				explicit encrypter( block_cipher &bc, const memory::ro_buffer &iv);
				virtual ~encrypter() throw();
				
				virtual void        crypt( void *output, const void *input ) throw();
				
			private:
				encrypter( const encrypter &);
				encrypter&operator=( const encrypter & );
			};
			
			//! \f$ P_i = D_K( C_i ) \f$
			class decrypter : public operating_block_cipher {
			public:
				//! rc must be the encrypter bc
				explicit decrypter( block_cipher &bc , block_cipher &rc, const memory::ro_buffer &iv );
				virtual ~decrypter() throw();
				
				virtual void        crypt( void *output, const void *input ) throw();
				
			private:
				decrypter( const decrypter &);
				decrypter&operator=( const decrypter & );
			};
			
			
		};
		
		
	}
	
}

#endif
