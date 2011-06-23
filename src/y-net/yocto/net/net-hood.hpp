//! \file net-hood.hpp

#ifndef YOCTO_NET_HOOD_INCLUDED
#define YOCTO_NET_HOOD_INCLUDED 1

#include "yocto/code/ilog2.hpp"
#include "yocto/fragment/queue.hpp"

namespace yocto {
	
	namespace network
	{
		
		//! data queue for flexible I/O
		/**
		 - When used with a TCP socket, it allows
		 to store the stream as it comes with a predefined block_size.
		 - When used with a UDP socket, each block is a raw Datagram,
		 so that block_size has to be tuned to fit the application blocks size.
		 */
		class dataQ : public fragment::queue
		{
		public:
			typedef uint64_t    align_type;
			static const size_t align_log2 = ilog2< sizeof(align_type) >::value;
			
			//! constructor
			/**
			 *  \param n required block_size for the fragment::dataQ
			 *  this->block_size will be aligned on sizeof(align_type)
			 */
			explicit dataQ( size_t n ) throw();
			virtual ~dataQ() throw(); //!< destructor
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN( dataQ );
		};
		
		
	}
	
}

#endif
