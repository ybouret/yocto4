#include "yocto/code/mtf.hpp"

#include <cstring>

namespace yocto {

	namespace core {

		move_to_front:: ~move_to_front() throw()
		{
			list_.reset();
		}

#if defined(_MSC_VER)
		// init of node_
#pragma warning ( disable : 4351 )
#endif
		move_to_front:: move_to_front() throw() :
		list_(),
		node_()
		{
			build();
		}


		void move_to_front::reset() throw() 
		{
			list_.reset();
			build();
		}

		void move_to_front:: build() throw() 
		{
			memset( node_, 0, sizeof(node_) );
			node_t *node = node_;
			int i=256;
			while( --i >= 0 ) {
				node->data = uint8_t(i);
				list_.push_front( node );
				++node;
			}
			assert( 256 == list_.size );
		}

		uint8_t move_to_front:: encode( uint8_t x ) throw()
		{
			assert(list_.size == 256 );
			node_t *node = list_.head;
			int     indx = 0;
			while( node->data != x ) {
				++indx;
				node = node->next;
				assert( node != NULL );
				assert( indx < 256   );
			}
			list_.move_to_front(node);
			assert(list_.size == 256 );
			return uint8_t(indx);
		}

		uint8_t move_to_front:: decode( uint8_t x ) throw() 
		{
			assert(list_.size == 256 );
			node_t *node = list_.head;
			for(register size_t i=x;i>0;--i) {
				node=node->next;
				assert(NULL!=node);
			}
			list_.move_to_front(node);
			return node->data;
		}



	}

}

