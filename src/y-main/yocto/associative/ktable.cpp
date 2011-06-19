#include "yocto/associative/ktable.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/primes.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
	
	namespace core
	{
		
		size_t ktable_num_slots_for( size_t &num_items)
		{
			assert(YOCTO_KTABLE_LOAD_FACTOR>1);
			if( num_items > 0 )
			{
				//-- compute #node
				size_t nodes = max_of<size_t>(YOCTO_KTABLE_MIN_SLOTS * YOCTO_KTABLE_LOAD_FACTOR,num_items);
				while( 0 != ( nodes % YOCTO_KTABLE_LOAD_FACTOR ) )
				{
					++nodes;
					if( nodes < num_items ) 
					{
						const size_t n = num_items;
						num_items = 0;
						throw libc::exception( ERANGE, "ktable nodes overflow@%u", unsigned(n) );
					}
				}
				
				//-- deduce #slots
				const size_t num_slots = next_prime( nodes/YOCTO_KTABLE_LOAD_FACTOR );
				
				//-- recompute #nodes
				const size_t ntmp = num_slots  * YOCTO_KTABLE_LOAD_FACTOR;
				if( ntmp < num_items ) 
				{
					const size_t n = num_items;
					num_items = 0;
					throw libc::exception( ERANGE, "ktable slots overflow@%u", unsigned(n) );
				}
				
				num_items = ntmp;
				return num_slots;
			}
			else return 0;
			
		}
		
	}
}
