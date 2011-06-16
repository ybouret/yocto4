#ifndef _YOCTO_BLOCK_CIPHER_OFB_
#define _YOCTO_BLOCK_CIPHER_OFB_ 1

#include "yocto/crypto/bc/operating-block-cipher.hpp"

namespace yocto {
	
	namespace crypto {
		
		
		//! Output FeedBack
		struct block_cipher_ofb {
			
			//! \f$ O_i = E_K(O_{i-1}), C_i = O_i \oplus P_i, O_0 = IV \f$
			class encrypter : public operating_block_cipher
				{
				public:
					explicit encrypter( block_cipher &bc, const memory::ro_buffer &iv);
					virtual ~encrypter() throw();
					
					virtual void        crypt( void *output, const void *input ) throw();
					virtual void        schedule( const memory::ro_buffer &IV ) throw();
					
				private:
					digest           O_;
					encrypter( const encrypter &);
					encrypter&operator=( const encrypter & );
				};
			
			//! \f$  O_i = E_K(O_{i-1}), P_i = O_i \oplus C_i, O_0 = IV \f$
			class decrypter : public operating_block_cipher {
			public:
				explicit decrypter( block_cipher &bc, const memory::ro_buffer &iv );
				virtual ~decrypter() throw();
				
				virtual void        crypt( void *output, const void *input ) throw();
				virtual void        schedule( const memory::ro_buffer &IV ) throw();
				
			private:
				digest         O_;
				decrypter( const decrypter &);
				decrypter&operator=( const decrypter & );
			};
			
			
		};
		
		
	}
	
}

#endif
