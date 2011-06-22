#ifndef YOCTO_FRAG_QUEUE_INCLUDED
#define YOCTO_FRAG_QUEUE_INCLUDED 1

#include "yocto/fragment/layout.hpp"
#include "yocto/fragment/block.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/code/ilog2.hpp"

namespace yocto
{
	
	namespace fragment
	{
		
		class queue : public layout
		{
		public:
			typedef  uint64_t       __unit;
			static   const size_t   __log2  = ilog2<sizeof(__unit)>::value;
			explicit queue(size_t user_block_size) throw();
			virtual ~queue() throw();
			
			void free() throw();
			void release() throw();
			
			
			size_t write( const void *buffer, size_t buflen );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(queue);
			size_t               bytes_; //!< total bytes in queue
			core::list_of<block> list_;
			core::pool_of<block> pool_;
			
			static void dispatch( void *frag_data, size_t frag_size, void *args);
			
		};
		
	}
}

#endif
