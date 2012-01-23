#ifndef YOCTO_CLIFF_GHOSTS_INCLUDED
#define YOCTO_CLIFF_GHOSTS_INCLUDED 1


// \file

#include "yocto/cliff/linear.hpp"
#include "yocto/sequence/vector.hpp"

#include <cstring>

namespace yocto
{
	
	namespace cliff
	{
		
		//! geometrical information
		enum ghost_position
		{
			ghost_lower_x = 0,
			ghost_upper_x = 1,
			ghost_lower_y = 2,
			ghost_upper_y = 3,
			ghost_lower_z = 4,
			ghost_upper_z = 5
		};
		
		const char * ghost_position_label( ghost_position p ) throw();
		
		
		//! base class for ghost: offsets and position
		class ghost_base
		{
		public:
			virtual ~ghost_base() throw();
			
			const offsets_list    offsets;   //!< where the workspace data are
			const ghost_position  position;  //!< for data I/O
			const bool            is_async;  //!< for MPI I/O and computation overlapping
 			const size_t          count;     //!< number of items (float,double,complex...)
			const size_t          bytes;     //!< bytes for this count
			const char           *label() const throw(); //!< from position
			
		protected:
			explicit ghost_base(size_t         max_offsets, 
								ghost_position pos, 
								bool           async) throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ghost_base);
		};
		
		//! number of ghosts and deferred type or not
		template <typename COORD>
		class ghosts_infos
		{
		public:
			COORD count; //!< count in each dimension
			COORD async; //!< not 0 => asynchronous
			
			inline ghosts_infos() throw() : count(), async() 
			{ 
				memset( (void*) &count, 0, sizeof(COORD) );
				memset( (void*) &async, 0, sizeof(COORD) );
			}
		
			inline ghosts_infos( const COORD &num, const COORD &status ) throw() :
			count( num ), async(status)
			{
			}
			
			inline ghosts_infos( const ghosts_infos &other ) throw() :
			count( other.count ),
			async( other.async )
			{
			}
			
			inline ghosts_infos & operator=( const ghosts_infos &other ) throw()
			{
				count = other.count;
				async = other.async;
				return *this;
			}
			
			inline ~ghosts_infos() throw() {}
		
			
		};
		
		//! info for lower and upper ghosts
		template <typename COORD>
		class ghosts_setup
		{
		public:
			typedef ghosts_infos<COORD> infos_type;
			const infos_type lower;
			const infos_type upper;
			
			inline ghosts_setup( const infos_type &lo, const infos_type &up ) throw() :
			lower(lo),
			upper(up)
			{
			}
			
			inline ghosts_setup( const ghosts_setup &other ) throw() :
			lower( other.lower ),
			upper( other.upper )
			{
			}
			
			inline ~ghosts_setup() throw() {}
			
		private:
			YOCTO_DISABLE_ASSIGN(ghosts_setup);
		};
		
		
		
		
		//! ghost
		/**
			Manage information for one ghost.
		 */
		template <typename T,typename COORD>
		class ghost :  public object, public layout<COORD>, public ghost_base
		{
		public:
			typedef typename layout<COORD>::param_coord param_coord;
			typedef linear<T,layout<COORD> >            linear_type;
			
			const size_t   nvar; //!< max number of storable variables, initially 0
		private:
			mutable T    **slot; //!< matrix [1..nvar][0..count-1]
		public:
			mutable int    peer; //!< MPI source/destination, initially -1
			
			//! create ghosts from coordinates and outline
			explicit ghost(ghost_position      pos, 
						   param_coord         lo, 
						   param_coord         hi ,
						   const layout<COORD> &outline,
						   const bool           async) :
			layout<COORD>( lo, hi ),
			ghost_base( this->items, pos, async ),
			nvar(0),
			slot(NULL),
			peer(-1)
			{
				assert( outline.has(this->lower) );
				assert( outline.has(this->upper) );
				
				//-- prepare offsets
				layout<COORD>::load_offsets( (offsets_list &)(this->offsets), outline, *this );
				
				//-- store info
				(size_t&)(this->count) = this->offsets.size();
				(size_t&)(this->bytes) = this->count * sizeof(T);
				
			}
			
			//! acquire data for asynchronous copy
			/**
				\param nvar > 0
				prepar nvar slots of this->count float/double/...
			 */
			void acquire_data( size_t num_var ) const
			{
				assert(num_var>0);
				assert(count>0);
				if( num_var != nvar )
				{
					release_data();
					size_t &nv = (size_t &)nvar;
					nv   = num_var;
					slot = memory::kind<memory::global>::acquire_as<T*>( nv )-1;
					try
					{
						size_t ndata = count * nvar;
						slot[1] = memory::kind<memory::global>::acquire_as<T>( ndata );
						for( size_t k=2; k <= nvar; ++k )
							slot[k] = slot[k-1] + count;
					}
					catch(...)
					{
						memory::kind<memory::global>::release_as<T*>( ++slot, nv );
						assert(NULL==slot);
						assert(0   ==nvar);
						throw;
					}
				}
			}
			
			//! release data
			void release_data() const
			{
				assert(count>0);
				if( nvar > 0 )
				{
					assert(slot!=NULL);
					size_t ndata = count * nvar;
					memory::kind<memory::global>::release_as<T>( slot[1], ndata );
					memory::kind<memory::global>::release_as<T*>( ++slot, (size_t&)nvar);
					assert( NULL == slot );
					assert( 0    == nvar );
				}
			}
			
			//! return a 0..count-1 of T
			T * operator[]( size_t ivar ) const throw()
			{
				assert(ivar>=1);
				assert(ivar<=nvar);
				return slot[ivar];
			}
			
			virtual ~ghost() throw() { release_data(); }
			
			//! pull data from source
			inline void pull( const linear_type &src, size_t ivar ) const throw()
			{
				assert(src.entry!=NULL);
				assert(slot!=NULL);
				assert(ivar>=1);
				assert(ivar<=nvar);
				
				const T *p = src.entry;
				T       *q = slot[ivar];
				for( size_t i = count; i >0; --i, ++q  )
				{
					const size_t j = offsets[i];
					assert(j<src.items);
					*q = p[j];
				}
				
			}
			
			//! push data into source
			inline void push( linear_type &src, size_t ivar ) const throw()
			{
				assert(src.entry!=NULL);
				assert(NULL!=slot);
				assert(ivar>=1);
				assert(ivar<=nvar);
				
				T       *p = src.entry;
				const T *q = slot[ivar];
				for( size_t i = count; i >0; --i  )
				{
					const size_t j = offsets[i];
					assert(j<src.items);
					p[j] = *(q++);
				}
			}
			
			//! direct data exchange
			static inline void direct_copy( const ghost &dst, const ghost &src, linear_type &lin ) throw()
			{
				assert(lin.entry!=NULL);
				assert(dst.count == src.count);
				T *p = lin.entry;
				for( size_t i = dst.count; i>0; --i )
				{
					const size_t into = dst.offsets[i];
					const size_t from = src.offsets[i];
					p[into] = p[from];
				}
			}
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
		};
		
		
		
	}
	
}

#endif

