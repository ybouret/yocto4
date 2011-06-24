#ifndef YOCTO_FRAGMENT_LAYOUT_INCLUDED
#define YOCTO_FRAGMENT_LAYOUT_INCLUDED 1


#include "yocto/os.hpp"


namespace yocto
{
	
	namespace fragment
	{
		class layout
		{
		public:
			static const size_t round_log2_min = 0x01; //!< => 2^1  => 2    bytes alignment
			static const size_t round_log2_max = 0x0A; //!< => 2^10 => 1024 bytes alignment
			
			explicit layout( size_t user_block_size, size_t user_round_log2 ) throw();
			virtual ~layout() throw();
			layout( const layout & ) throw();
			
			const size_t round_log2; /*!< clamped                   */
			const size_t round_byte; /*!< 2^round_log2_             */
			const size_t round_mask; /*!< round_byte_-1             */
			const size_t round_not2; /*!< ~roundMask_               */
			const size_t block_size; /*!< final fragment block size */
			
			size_t round( size_t n ) const throw();
			
			typedef void (*callback)( void *frag_data, size_t frag_size, void *args ); 
			
			//! successive call to callback with optimized block sizes
			/**
			 *	avoid small blocks, always aligned to round_bytes,
			 *  always full or more than half full.
			 */
			void process( void *huge_data, size_t data_size, callback cb_proc, void *cb_args) const;
			
		private:
			YOCTO_DISABLE_ASSIGN(layout);
		};
		
	}
	
}

#endif
