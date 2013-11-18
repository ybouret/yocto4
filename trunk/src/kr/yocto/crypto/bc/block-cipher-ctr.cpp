#include "yocto/crypto/bc/block-cipher-ctr.hpp"
#include <cstring>

namespace yocto {

	namespace crypto {



		block_cipher_ctr:: encrypter:: encrypter( block_cipher &bc, const memory::ro_buffer &iv) :
				operating_block_cipher( bc, bc, iv ),
				CTR_( size_, 0 )
		{
			CTR_ = iv;
		}


		block_cipher_ctr:: encrypter:: ~encrypter() throw()
		{
		}

		void block_cipher_ctr:: encrypter:: crypt( void *output, const void *input ) throw() {

			memcpy( P_.rw(), input, size_ );
			bc_.crypt( R_.rw(), CTR_.ro() );
			C_._xor( R_, P_ );
			CTR_._inc(1);
			memcpy( output, C_.ro(), size_ );

			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}

		void block_cipher_ctr:: encrypter:: schedule( const memory::ro_buffer &IV ) throw()
		{
			this->operating_block_cipher::schedule(IV);
			CTR_ = IV;
		}
		


		block_cipher_ctr:: decrypter:: decrypter( block_cipher &bc, const memory::ro_buffer &iv ) :
				operating_block_cipher( bc, bc, iv ),
				CTR_( size_, 0 )
		{
			CTR_ = iv;
		}

		
		block_cipher_ctr:: decrypter:: ~decrypter() throw()
		{
		}

		void block_cipher_ctr:: decrypter:: crypt( void *output, const void *input ) throw() {
			memcpy( C_.rw(), input, size_ );
			bc_.crypt( R_.rw(), CTR_.ro() );
			P_._xor( R_, C_ );
			CTR_._inc(1);
			memcpy( output, P_.ro(), size_ );

			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}

		void block_cipher_ctr:: decrypter:: schedule( const memory::ro_buffer &IV ) throw()
		{
			this->operating_block_cipher::schedule(IV);
			CTR_ = IV;
		}
		
	}

}
