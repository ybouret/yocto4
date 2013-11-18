#ifndef YOCTO_MEMORY_SLAB_INCLUDED
#define YOCTO_MEMORY_SLAB_INCLUDED 1


#include "yocto/code/ilog2.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
	
	namespace memory 
	{
		
		class slab 
		{
		public:
			static const size_t bs_round_ln2   = ilog2<sizeof(size_t)>::value;
			
			static size_t bytes_for( size_t block_size, size_t num_blocks );
			static size_t round_bs( size_t block_size ) throw();
			
			~slab() throw();
			slab( void *entry, const size_t block_size, const size_t num_blocks ) throw();
			
            void reset() throw(); //!< forced reset
            
            //! forced reformatting
            void format(void         *entry,
                        const size_t block_size,
                        const size_t num_blocks ) throw();
            
            
			void *acquire() throw();
			void  release(void *) throw();
			
			uint8_t      *data;
			size_t        firstAvailable; //!< data index
			size_t        stillAvailable; //!< bookeeping
			
			const size_t  blockSize;
			const size_t  numBlocks; //!< inital num_blocks
			
		private:
            void link() throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(slab);
		};
		
		template <typename T>
		class slab_of
		{
		public:
			static inline size_t bytes_for( size_t num_blocks ) { return slab::bytes_for( sizeof(T), num_blocks ); }
			
			inline  slab_of( void *entry, const size_t num_blocks ) throw() : slab_( entry, sizeof(T), num_blocks ) {}
			inline ~slab_of() throw() {}
			
			inline T   *query() throw() { return static_cast<T *>( slab_.acquire() ); }
			inline void store( T *p) throw() { slab_.release(p); }
			
			inline size_t capacity()  const throw() { return slab_.numBlocks;      }
			inline size_t available() const throw() { return slab_.stillAvailable; }
			inline size_t involved()  const throw() { return capacity() - available(); }
			
            inline void reset() throw() { slab_.reset(); }
            inline void format(void *entry, const size_t num_blocks) throw() { slab_.format(entry,sizeof(T),num_blocks); }
            
            inline void swap_with( slab_of &other ) throw()
            {
                bswap( slab_, other.slab_ );
            }
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(slab_of);
			slab slab_;
		};
		
	}
	
}

#endif
