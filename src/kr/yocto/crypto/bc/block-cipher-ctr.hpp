#ifndef _YOCTO_BLOCK_CIPHER_CTR_
#define _YOCTO_BLOCK_CIPHER_CTR_ 1

#include "yocto/crypto/bc/operating-block-cipher.hpp"

namespace yocto {
	
	namespace crypto {
		
		//! CounTeR Block Cipher
		struct block_cipher_ctr {
			
			//! \f$ C_i = E_K(CTR_{i-1}) \oplus P_i, CTR_i = CTR_{i-1}+1, CTR_0 = IV \f$
			class encrypter : public operating_block_cipher
				{
				public:
					explicit encrypter( block_cipher &bcph, const memory::ro_buffer &iv);
					virtual ~encrypter() throw();
					
					virtual void        crypt( void *output, const void *input ) throw();
					virtual void        schedule( const memory::ro_buffer &IV ) throw();
					
				private:
					digest           CTR_;
					encrypter( const encrypter &);
					encrypter&operator=( const encrypter & );
				};
			
			//! \f$  P_i = E_K(CTR_{i-1}) \oplus C_i, CTR_{i} = CTR_{i-1}+1, CTR_0 = IV \f$
			class decrypter : public operating_block_cipher {
			public:
				explicit decrypter( block_cipher &bcph, const memory::ro_buffer &iv );
				virtual ~decrypter() throw();
				
				virtual void        crypt( void *output, const void *input ) throw();
				virtual void        schedule( const memory::ro_buffer &IV ) throw();
				
			private:
				digest         CTR_;
				decrypter( const decrypter &);
				decrypter&operator=( const decrypter & );
			};
			
			
		};
		
		
	}
	
}

#endif
