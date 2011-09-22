#ifndef YOCTO_SPADE_LINEAR_INCLUDED
#define YOCTO_SPADE_LINEAR_INCLUDED 1

#include "yocto/spade/layout.hpp"
#include "yocto/code/unroll.hpp"

#include <cstring>

namespace yocto
{
	
	namespace spade
	{
		
		template <typename T, typename LAYOUT>
		class linear : public LAYOUT
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			typedef LAYOUT layout;
			const size_t bytes;
			T           *entry; //!< must be linked to an array of LAYOUT::items
			
			
			virtual ~linear() throw() {}
			
			explicit linear( const LAYOUT &L ) throw() :
			LAYOUT( L ),
			bytes( this->items * sizeof(T) ),
			entry( NULL )
			{
			}
			
			virtual void request( size_t lengths[] ) const throw() = 0;
			virtual void link_to( void * buffers[] ) throw() = 0;
			
			
			inline void ldz() throw()
			{
				assert( entry );
				memset( entry, 0, bytes );
			}
			
			inline void ld( param_type value ) throw()
			{
				assert( entry );
				T *p = entry;
				YOCTO_LOOP_( this->items, *(p++) = value );
			}
			
			inline T    compute_min( size_t *offset = NULL ) const throw()
			{
				assert( entry );
				T      the_min = entry[ 0 ];
				size_t min_idx = 0;
				for( size_t i=1; i < this->items; ++i )
				{
					const T tmp = entry[ i ];
					if( tmp < the_min )
					{
						the_min = tmp;
						min_idx = i;
					}
				}
				if( offset ) *offset = min_idx;
				return the_min;
			}
			
			T    compute_max( size_t *offset = NULL ) const throw()
			{
				assert( entry );
				T      the_max = entry[ 0 ];
				size_t max_idx = 0;
				for( size_t i=1; i < this->items; ++i )
				{
					const T tmp = entry[ i ];
					if( tmp > the_max )
					{
						the_max = tmp;
						max_idx = i;
					}
				}
				if( offset ) *offset = max_idx;
				return the_max;
			}
			
			void compute_min_max( T &the_min, T &the_max, size_t *off_min=NULL, size_t *off_max=NULL ) const throw()
			{
				assert( entry );
				the_max = entry[ 0 ];
				the_min = entry[0];
				size_t max_idx = 0,          min_idx = 0;
				for( size_t i=1; i < this->items; ++i )
				{
					const T tmp = entry[ i ];
					if( tmp > the_max )
					{
						the_max = tmp;
						max_idx = i;
					}
					else {
						if( tmp < the_min )
						{
							the_min = tmp;
							min_idx = i;
						}
					}
				}
				if( off_min ) *off_min = min_idx;
				if( off_max ) *off_max = max_idx;
				
			}
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
		};
		
		
	}
	
}

#endif
