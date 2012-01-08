#ifndef YOCTO_CORE_LOCATE_INCLUDED
#define YOCTO_CORE_LOCATE_INCLUDED 1


#include "yocto/os.hpp"
#include <cstring>

namespace yocto
{
	
	
	namespace core
	{
		/**! locate the index */
		/**
		 \param  target  LHS argument to compare
		 \param  base    base address of and ordered array [0,size-1]
		 \param  size    the number of items.
		 \param  indx    where to insert if failure, location if success
		 \param  compare comparison functionoid
		 \return false if failure,  true if success.
		 
		 comparison are performed with user_proc( target, item_base+something, user_data),
		 so that target can be compared to whatever item_base is pointing at.
		 
		 */
		
		template <typename U, typename T, typename CMP>
		inline bool locate( U *target, T *base, const size_t size, size_t &indx, CMP compare )
		{
			assert(!(NULL==base&&size>0) );
			if( size > 0 )
			{
				T *lo = base;
				T *hi = base+size;
				--hi;
				while( lo <= hi )
				{
					T        *mid = &lo[static_cast<size_t>(hi-lo)>>1];
					
					const int ans = compare(*target,*mid);
					if( ans < 0 )
					{
						hi = mid-1;
					}
					else {
						if( ans > 0 )
						{
							lo=mid+1;
						}
						else 
						{
							indx = static_cast<size_t>(mid-base);
							return true;
						}
					}
					
				}
				indx = static_cast<size_t>(lo-base);
				return false;
			}
			else {
				indx = 0;
				return false;
			}
			
		}
		
		/** memory insertion */
		/**
		 \param  base    base address of and ordered array [0,size (at least) ]
		 \param  size    the number of items, UNTOUCHED
		 \param  indx    where to insert 
		 */
		template <typename T>
		inline void insert( const T *target, T *base, const size_t size, const size_t indx ) throw()
		{
			assert(indx<=size);
			T *dst = base + indx;
			memmove( (void*)(dst+1),  (void*)dst, (size-indx) * sizeof(T) );
			memcpy(  (void*)dst,   (void*)target,               sizeof(T) ); 
		}
		
	}
	
}

#endif
