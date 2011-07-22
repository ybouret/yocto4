#ifndef YOCTO_META_LIST_INCLUDED
#define YOCTO_META_LIST_INCLUDED 1


#include "yocto/core/list.hpp"


namespace yocto
{
	
	namespace core
	{
		//! assume cloneable node
		template <typename NODE>
		class meta_list : public list_of<NODE>
		{
		public:
			explicit meta_list() throw() {}
			virtual ~meta_list() throw() { kill(); }
			meta_list( const meta_list &other ) 
			{
				try
				{
					for( const NODE *node = other.head; node; node=node->next )
					{
						this->push_back( node->clone() );
					}
					
				}
				catch(...) { kill(); throw; }
			}
			
			inline void kill() throw() { while(this->size) delete this->pop_back(); }
			
		private:
			YOCTO_DISABLE_ASSIGN(meta_list);
		};
	}
}


#endif
