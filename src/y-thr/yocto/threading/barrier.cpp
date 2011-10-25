#include "yocto/threading/barrier.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	
	namespace threading
	{
		
		barrier:: barrier( size_t threshold_value, const char *id) throw() :
		threshold( max_of<size_t>( threshold_value, 1 ) ),
		counter( threshold ),
		cycle(0),
		guard(id),
		cv()
		{
		}
		
		barrier:: ~barrier() throw() {}
		
		
		const char *barrier:: name() const throw() { return guard.name(); }
		
		
		bool barrier:: wait() throw()
		{
			bool status = false;
			guard.lock();
			{
				const size_t local_cycle = cycle;
				if( --counter <= 0 )
				{
					++cycle;
					counter = threshold;
					cv.broadcast();
					status = true;
				}
				else
				{
					while( local_cycle == cycle )
					{
						cv.wait( guard );
					}
					
				}
			}
			guard.unlock();
			return status;
		}
		
		
	}
	
}
