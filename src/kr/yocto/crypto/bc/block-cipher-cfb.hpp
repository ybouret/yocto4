#ifndef _YOCTO_BLOCK_CIPHER_CFB_
#define _YOCTO_BLOCK_CIPHER_CFB_ 1

#include "yocto/crypto/bc/operating-block-cipher.hpp"

namespace yocto {

	namespace crypto {


		//! Cipher FeedBack
		struct block_cipher_cfb {

			//! \f$ C_i = E_K(C_{i-1}) \oplus P_i, C_0 = IV \f$
			class encrypter : public operating_block_cipher 
				{
				public:
					explicit encrypter( block_cipher &bc, const memory::ro_buffer &iv);
					virtual ~encrypter() throw();

					virtual void        crypt( void *output, const void *input ) throw();

				private:
					encrypter( const encrypter &);
					encrypter&operator=( const encrypter & );
			};

			//! \f$ P_i = E_K(C_{i-1}) \oplus C_{i}, C_0=IV \f$
			class decrypter : public operating_block_cipher 
				{
				public:
					explicit decrypter( block_cipher &bc, const memory::ro_buffer &iv );
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
