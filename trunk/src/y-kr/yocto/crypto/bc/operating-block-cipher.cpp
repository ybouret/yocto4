#include "yocto/crypto/bc/operating-block-cipher.hpp"

namespace yocto {
	
	namespace crypto {
		
		operating_block_cipher:: ~operating_block_cipher() throw() 
		{
		}
		
		
		void operating_block_cipher:: crypt_flush( void *output, const void *input, const std::size_t len ) throw() 
		{
			assert(len<this->size());
			
			if( len > 0 )
			{
				
				//-- build mask with residual cipher
				rc_.crypt( R_.rw(), Cp_.ro() );
				
				//-- emit message
				uint8_t       *tgt = (uint8_t *)output;
				const uint8_t *src = (const uint8_t *)input;
				const uint8_t *msk = &R_[0];
				for( size_t i=len;i>0;--i)
				{
					*(tgt++) = uint8_t( (*(src++)) ^ (*(msk++)) );
				}
				
			}
			
		}
		
		operating_block_cipher:: operating_block_cipher( block_cipher &bcph, block_cipher &rcph, const  memory::ro_buffer &IV ) :
		bc_( bcph ),
		rc_( rcph ),
		size_( bc_.size() ),
		P_(   size_, 0 ),
		C_(   size_, 0 ),
		Pp_(  size_, 0 ),
		Cp_(  size_, 0 ),
		R_(   size_, 0 )
		{
			assert( bc_.size() == rc_.size() );
			Pp_ = IV; // truncate
			Cp_ = IV; // truncate
		}
		
		
		std::size_t operating_block_cipher::size() const throw() 
		{
			return size_;
		}
		
		void operating_block_cipher:: schedule( const memory::ro_buffer &IV ) throw()
		{
			P_._set(0);
			C_._set(0);
			R_._set(0);
			Pp_ = IV;
			Cp_ = IV;
		}
		
		void operating_block_cipher:: scramble( void *buf, size_t len ) throw()
		{
			assert( !(buf==NULL&&len>0) );
			this->crypt_block(buf,buf,len);
		}
		
		void operating_block_cipher:: crypt_block( void *output, const void *input, const std::size_t length ) throw()
		{
			
			uint8_t       *tgt = (uint8_t *)output;
			const uint8_t *src = (const uint8_t *)input;
			std::size_t    num = length;
			
			while( num >= size_ ) 
			{
				this->crypt( tgt, src ); //!-- full
				tgt += size_;
				src += size_;
				num -= size_;
			}
			
			assert( num < size_ );
			this->crypt_flush( tgt, src, num );
		}
		
	}
	
}
