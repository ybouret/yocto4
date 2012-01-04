#ifndef YOCTO_CLIFF_LINEAR_INCLUDED
#define YOCTO_CLIFF_LINEAR_INCLUDED 1


#include "yocto/cliff/layout.hpp"


namespace yocto
{
	
	namespace cliff
	{
		template <typename T, typename LAYOUT>
		class linear : public LAYOUT
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			const size_t bytes;
			T           *entry; //!< must be linked to an array of LAYOUT::items
			
			
			virtual ~linear() throw() {}
			
			explicit linear( const LAYOUT &L ) throw() :
			LAYOUT( L ),
			bytes( this->items * sizeof(T) ),
			entry( NULL )
			{
			}
			
			//! link to data, dimension dependent
			virtual void link( T *addr ) throw() = 0;
			
			//! set every item to 0
			inline void ldz() throw() { assert(entry); memset( entry, 0, bytes); }
			
			//! set every item to the same value
			inline void ld(param_type value) throw()
			{
				assert(entry);
				type *p = entry;
				for( size_t i=this->items; i>0;--i )
				{
					*(p++) = value;
				}
			}
			
			//! find the minimum value (float/double)
			inline type get_min( unit_t *offset  ) const throw()
			{
				assert(entry);
				unit_t      imin = 0;
				type        vmin = entry[0];
				for( size_t i = 1; i < this->items; ++i )
				{
					const type vtmp = entry[i];
					if( vtmp < vmin )
					{
						vmin = vtmp;
						imin = i;
					}
				}
				if(offset) *offset = imin;
				return vmin;
			}
			
			//! find the maximum value (float/double)
			inline type get_max( unit_t *offset  ) const throw()
			{
				assert(entry);
				unit_t      imax = 0;
				type        vmax = entry[0];
				for( size_t i = 1; i < this->items; ++i )
				{
					const type vtmp = entry[i];
					if( vtmp > vmax )
					{
						vmax = vtmp;
						imax = i;
					}
				}
				if(offset) *offset = imax;
				return vmax;
			}
			
			inline void get_min_max( type &vmin, unit_t *offmin, type &vmax, unit_t *offmax ) const throw()
			{
				assert(entry);
				unit_t imin = 0;
				unit_t imax=0;
				vmin = entry[0];
				vmax = entry[0];
				
				for( size_t i = 1; i < this->items; ++i )
				{
					const type vtmp = entry[i];
					if( vtmp > vmax )
					{
						vmax = vtmp;
						imax = i;
					}
					else {
						if( vtmp < vmin )
						{
							vmin = vtmp;
							imin = i;
						}
					}

				}
				if(offmax) *offmax = imax;
				if(offmin) *offmin = imin;
			}
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
		};
	}
}

#endif
