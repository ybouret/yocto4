#ifndef YOCTO_CLIFF_LINEAR_INCLUDED
#define YOCTO_CLIFF_LINEAR_INCLUDED 1


#include "yocto/cliff/layout.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		//! linear space  on a layout
		template <typename T, typename LAYOUT>
		class linear : public LAYOUT
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			const size_t bytes; //!< linear bytes
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
			
			typedef void (*callback_type)(type&,void*);
			typedef void (*const_cb_type)(const_type&,void*);
			typedef void (*callback2_type)(type &,const type &,void *);
			
			virtual void foreach( const LAYOUT &sub, callback_type  proc, void *args) = 0;
			virtual void foreach( const LAYOUT &sub, const_cb_type  proc, void *args) const  = 0;
			
			inline void set_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, set_cb, (void*)&v); }
			inline void add_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, add_cb, (void*)&v); }
			inline void mul_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, mul_cb, (void*)&v); }									
			inline type sum( const LAYOUT &sub ) throw() { type ans(0); foreach( sub, sum_cb, &ans); return ans; }
			
			inline void save( ios::ostream &fp, const LAYOUT &sub) const { foreach( sub, save_cb, &fp); }
			inline void load( ios::istream &fp, const LAYOUT &sub)       { foreach( sub, load_cb, &fp); }
			
		protected:
			static inline void set_cb( type &v, void *args) throw() { v  = *(type*)args; }
			static inline void add_cb( type &v, void *args) throw() { v += *(type*)args; }
			static inline void mul_cb( type &v, void *args) throw() { v *= *(type*)args; }
			static inline void sum_cb( const_type &v, void *args) throw() { *(type *)args += v; }
			
			static inline void set2_cb( type &v, const_type &u, void *) throw() { v = u; }
			static inline void add2_cb( type &v, const_type &u, void *) throw() { v += u; }
			static inline void muladd_cb( type &v,const_type &u, void *args) throw() { v += (*(const_type *)args) * u; }
			
			static inline void save_cb( const_type &v, void *args )
			{
				ios::ostream &fp = *(ios::ostream *)args;
				fp.save( &v, sizeof(type) );
			}
			
			static inline void load_cb( type &v, void *args )
			{
				ios::istream &fp = *(ios::istream *)args;
				fp.load( &v, sizeof(type) );
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
		};
	}
}

#endif
