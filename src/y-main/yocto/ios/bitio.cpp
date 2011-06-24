#include "yocto/ios/bitio.hpp"

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
		
	}
	
}
