#include "yocto/crypto/bc/block-cipher-cfb.hpp"
#include <cstring>

namespace yocto {

	namespace crypto {



		block_cipher_cfb:: encrypter:: encrypter( block_cipher &bc, const memory::ro_buffer &iv) :
				operating_block_cipher( bc, bc, iv )
		{
		}

		

		block_cipher_cfb:: encrypter:: ~encrypter() throw()
		{
		}

		void block_cipher_cfb:: encrypter:: crypt( void *output, const void *input ) throw() {

			memcpy( P_.rw(), input, size_ );
			bc_.crypt( C_.rw(), Cp_.ro() );
			C_._xor( P_ );
			memcpy( output, C_.ro(), size_ );

			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}




		block_cipher_cfb:: decrypter:: decrypter( block_cipher &bc, const memory::ro_buffer &iv ) :
				operating_block_cipher( bc, bc, iv )
		{
		}

		block_cipher_cfb:: decrypter:: ~decrypter() throw()
		{
		}

		void block_cipher_cfb:: decrypter:: crypt( void *output, const void *input ) throw() 
		{
			memcpy( C_.rw(), input, size_ );
			bc_.crypt( R_.rw(), Cp_.ro() );
			P_._xor( R_, C_ );
			memcpy( output, P_.ro(), size_ );
			
			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}

	}

}
