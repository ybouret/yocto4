#ifndef _YOCTO_OP_CIPHER_HPP_
#define _YOCTO_OP_CIPHER_HPP_ 1

#include "yocto/crypto/bc/block-cipher.hpp"
#include "yocto/memory/blender.hpp"

namespace yocto {


	namespace crypto {


		//! the crypt operation is still virtual !
		class operating_block_cipher : public block_cipher, public memory::blender
		{
		public:

			virtual ~operating_block_cipher() throw();

			virtual size_t size() const throw();
			
			//! residual crypting, 0<=len<size()
			void crypt_flush( void *output, const void *input, const size_t len ) throw();
			
			//! helper for any block
			void crypt_block( void *output, const void *input, const size_t len ) throw();
			
			//! blender interface
			virtual void scramble( void *buffer, size_t length ) throw();
			virtual void schedule( const memory::ro_buffer &IV ) throw();
			
						
		protected:
			explicit operating_block_cipher( block_cipher &bcph, block_cipher &rcph, const memory::ro_buffer &IV );

			block_cipher     &bc_;   //!< full block cipher
			block_cipher     &rc_;   //!< residual bock cipher
			const size_t      size_; //!< common size
			digest            P_;    //!< Plain[i]
			digest            C_;    //!< Crypt[i]
			digest            Pp_;   //!< Plain[i-1]
			digest            Cp_;   //!< Crypt[i-1]
			digest            R_;    //!< residual helper



		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(operating_block_cipher);
			

		};

	}

}

#endif
