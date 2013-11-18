#ifndef YOCTO_PACK_MTF_INCLUDED
#define YOCTO_PACK_MTF_INCLUDED 1

#include "yocto/core/list.hpp"

namespace yocto
{
	
	namespace pack
	{
		class move_to_front 
		{
		public:
			explicit move_to_front() throw();
			virtual ~move_to_front() throw();
			uint8_t  encode( uint8_t ) throw();
			uint8_t  decode( uint8_t ) throw();
			void     reset(void)   throw();
            
            //! possible output=input
			void     encode(void *output,const void *input, size_t length) throw();
            
            //! possible output=input
            void     decode(void *output,const void *input, size_t length) throw();

            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(move_to_front);
			struct node_t
            {
				node_t *next;
				node_t *prev;
				uint8_t data;
			};
			core::list_of<node_t>  list_;
			node_t                 node_[256];
			void build() throw();
		};
		
		
	}
}


#endif
