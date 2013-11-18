#include "yocto/crypto/bc/block-cipher-ofb.hpp"
#include <cstring>

namespace yocto {

	namespace crypto {



		block_cipher_ofb:: encrypter:: encrypter( block_cipher &bc, const memory::ro_buffer &iv) :
				operating_block_cipher( bc, bc, iv ),
				O_( size_, 0 )
		{
			O_ = iv;
		}

		block_cipher_ofb:: encrypter:: ~encrypter() throw()
		{
		}

		void block_cipher_ofb:: encrypter:: crypt( void *output, const void *input ) throw() {

			memcpy( P_.rw(), input, size_ );
			bc_.crypt( R_.rw(), O_.ro() );
			C_._xor( P_, R_ );
			O_._swp( R_ );
			memcpy( output, C_.ro(), size_ );

			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}

		void block_cipher_ofb:: encrypter:: schedule( const memory::ro_buffer &IV ) throw()
		{
			this->operating_block_cipher::schedule( IV );
			O_ = IV;
		}


		block_cipher_ofb:: decrypter:: decrypter( block_cipher &bc, const memory::ro_buffer &iv ) :
				operating_block_cipher( bc, bc, iv ),
				O_( size_, 0 )
		{
			O_ = iv;
		}

		block_cipher_ofb:: decrypter:: ~decrypter() throw()
		{
		}

		void block_cipher_ofb:: decrypter:: crypt( void *output, const void *input ) throw() 
		{
			memcpy( C_.rw(), input, size_ );
			bc_.crypt( R_.rw(), O_.ro() );
			P_._xor( C_, R_ );
			O_._swp( R_ );
			memcpy( output, P_.ro(), size_ );

			C_._swp( Cp_ );
			P_._swp( Pp_ );
		}
		
		void block_cipher_ofb:: decrypter:: schedule( const memory::ro_buffer &IV ) throw()
		{
			this->operating_block_cipher::schedule( IV );
			O_ = IV;
		}

	}

}
