#ifndef _YOCTO_TEAC_HPP_
#define _YOCTO_TEAC_HPP_ 1

#include "yocto/crypto/bc/block-cipher.hpp"
#include "yocto/crypto/bc/tea.hpp"
#include <cstring>

namespace yocto {
	
	namespace crypto {
		
		class tea_cipher : public block_cipher
			{
			public:
				virtual ~tea_cipher() throw();
				
				virtual size_t size() const throw();
				
			protected:
				explicit tea_cipher( size_t block_size , const memory::ro_buffer &k );
				
				digest            k128_; //!< 128 bits key for tea_[encrypt|decrypt]
				const size_t      size_; //!< tea block size, >=8 and aligned on 4
				const size_t      nU32_; //!< tea dw_num
				digest            rU32_; //!< aligned
				
			private:
				tea_cipher( const tea_cipher & );
				tea_cipher&operator=( const tea_cipher & );
				
			};
		
		
		//!
		/**
		 BITS >= 64
		 */
		template <
		const size_t               BITS,
		const block_cipher::action MODE
		>
		class teac : public tea_cipher {
		public:
			virtual ~teac() throw()
			{
			}
			
			explicit teac( const memory::ro_buffer &k ) :
			tea_cipher( BITS/8, k ),
			wksp_( (uint32_t *)rU32_.rw() ),
			kr32_( (const uint32_t *)k128_.ro())
			{
				
			}
			
			virtual void crypt( void *output, const void *input ) throw() 
			{
				memcpy( wksp_,  input, size_ );
				tea_exec( int2type<MODE>() );
				memcpy( output, wksp_, size_ );
			}
			
			
		private:
			uint32_t       *wksp_;
			const uint32_t *kr32_;
			
			teac( const teac & );
			teac&operator=( const teac & );
			
			inline
			void tea_exec(int2type<encrypting> ) throw() {
				tea_encrypt( wksp_, nU32_,  kr32_ );
			}
			
			inline
			void tea_exec( int2type<decrypting> ) throw() {
				tea_decrypt( wksp_, nU32_,  kr32_ );
			}
			
			
		};
		
		template <const size_t BITS>
		struct teac_of
		{
			typedef teac<BITS,block_cipher::encrypting> encrypter;
			typedef teac<BITS,block_cipher::decrypting> decrypter;
		};
		
		
	} //crypto
	
} //yocto




#endif
