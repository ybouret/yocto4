#ifndef _YOCTO_BLOCK_CIPHER_PCBC_
#define _YOCTO_BLOCK_CIPHER_PCBC_ 1

#include "yocto/crypto/bc/operating-block-cipher.hpp"

namespace yocto {
	
	namespace crypto {
		
		
		//! Propagating Chained Block Cipher
		struct block_cipher_pcbc {
			
			//! \f$ C_i = E_K( P_i \oplus P_{i-1} \oplus C_{i-1}), P_0 \oplus C_0 = IV \f$
			class encrypter : public operating_block_cipher {
			public:
				explicit encrypter( block_cipher &bc, const memory::ro_buffer &iv);
				virtual ~encrypter() throw();
				
				virtual void        crypt( void *output, const void *input ) throw();
				
			private:
				encrypter( const encrypter &);
				encrypter&operator=( const encrypter & );
			};
			
			//! \f$ P_i = D_K(C_i) \oplus P_{i-1} \oplus C_{i-1}, P_0 \oplus C_0=IV \f$
			class decrypter : public operating_block_cipher {
			public:
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
