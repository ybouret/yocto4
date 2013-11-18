#ifndef YOCTO_XARRAY_INCLUDED 
#define YOCTO_XARRAY_INCLUDED 1

#include "yocto/memory/slab.hpp"
#include "yocto/memory/embed.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
	namespace core
	{
		template <typename SLOT>
		class xarray
		{
		public:
			typedef SLOT                 *SLOT_PTR;
			typedef memory::slab_of<SLOT> slab_t; 
			
			SLOT_PTR      *table; //!< stored
			slab_t         slots; //!< memory
			
			inline  xarray() throw() : table(NULL), slots(NULL,0),buffer_(NULL),buflen_(0) {}
			inline ~xarray() throw() { assert( slots.involved() == 0 ); }
			
			inline xarray( size_t n, memory::allocator &alloc ) : table(NULL), slots(NULL,0), buffer_(NULL), buflen_(0)
			{
				if(n>0)
				{
					void *entry = NULL;
					memory::embed reg[] =
					{
						{ (void **)(void*)&table, n * sizeof(SLOT_PTR), 0 },
						{                 &entry, slab_t::bytes_for(n), 0 }
					};
					const size_t num = sizeof(reg)/sizeof(reg[0]);
					buflen_ = memory::embed::prepare( reg, num );
					buffer_ = alloc.acquire( buflen_ ); 
					memory::embed::dispatch( reg, num, buffer_ );
					slab_t tmp_slots( entry, n ); yocto::bswap( tmp_slots, slots );
				}
			}
			
			inline void release_all( memory::allocator &alloc ) throw()
			{
				assert( slots.involved() == 0 );
				alloc.release(buffer_,buflen_);
				table = NULL;
				slab_t tmp_slots( NULL, 0 ); yocto::bswap( tmp_slots, slots );
			}
			
			inline size_t bytes() const throw() { return buflen_; }
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(xarray);
			void  *buffer_;
			size_t buflen_;
		};
		
	}
	
}


#endif
