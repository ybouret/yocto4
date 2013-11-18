#include "yocto/ios/bitio.hpp"
#include <iostream>

namespace yocto {
	
	
	namespace ios {
		
		bitio:: bitio() throw()
		{
		}
		
		
		bitio:: bitio( size_t n, const as_capacity_t &) :
		list<bool>( n, as_capacity )
		{
		}
		
		bitio:: ~bitio() throw()
		{
			
		}
		
		void bitio:: push( bool b )
		{
			push_back( b );
		}
		
		bool bitio:: pop() throw()
		{
			assert( list_.size > 0 );
			const bool ans = list_.head->data;
			pool_.store( list_.pop_front() );
			return ans;
		}
		
		bool bitio:: peek() const throw()
		{
			assert( list_.size > 0 );
			return list_.head->data;
		}
		
		void bitio:: skip() throw()
		{
			assert( list_.size > 0 );
			pool_.store( list_.pop_front() );
		}
		
		void bitio:: skip( size_t n ) throw()
		{
			assert( list_.size >= n );
			while( n-- > 0 )
				pool_.store( list_.pop_front() );
		}
		
		void bitio:: fill_to_byte_with( bool b )
		{
			const size_t mark = list_.size;
			try
			{
				while( 0 != (list_.size & 7) )
					push_back( b );
				
				assert( 0 == ( size() & 7 ) );
			}
			catch(...)
			{
				while( list_.size > mark ) pop_back();
				throw;
			}
		}
		
        void bitio:: output( std::ostream &os, size_t nbits) const
        {
            assert( nbits <= list_.size );
            const node_type *node = list_.head;
            for( size_t i=0; i < nbits; ++i, node=node->next )
            {
                os << ( node->data ? "1" : "0" );
            }
        }

	}
	
}
