#include "yocto/crypto/bc/block-cipher-pcbc.hpp"
#include <cstring>

namespace yocto {

	namespace crypto {



		block_cipher_pcbc:: encrypter:: encrypter( block_cipher &bc, const memory::ro_buffer &iv) :
				operating_block_cipher( bc, bc, iv )
		{
		}

		
		block_cipher_pcbc:: encrypter:: ~encrypter() throw()
		{
		}

		void block_cipher_pcbc:: encrypter:: crypt( void *output, const void *input ) throw() 
		{
			memcpy( P_.rw(), input, size_ );
			R_._set( P_ );
			R_._xor( Cp_ );
			R_._xor( Pp_ );

			bc_.crypt( C_.rw(), R_.ro() );
			memcpy( output, C_.ro(), size_ );
			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}




		block_cipher_pcbc:: decrypter:: decrypter( block_cipher &bc , block_cipher &rc ,const memory::ro_buffer &iv ) :
				operating_block_cipher( bc, rc, iv )
		{
		}

	
		block_cipher_pcbc:: decrypter:: ~decrypter() throw()
		{
		}

		void block_cipher_pcbc:: decrypter:: crypt( void *output, const void *input ) throw() 
		{
			memcpy( C_.rw(), input, size_ );
			bc_.crypt( R_.rw(), C_.ro() );

			R_._xor( Cp_ );
			P_._xor( R_, Pp_ );

			memcpy( output, P_.ro(), size_ );

			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}

	}

}
