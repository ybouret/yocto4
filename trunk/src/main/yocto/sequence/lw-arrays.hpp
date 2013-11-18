#ifndef YOCTO_LW_ARRAYS_INCLUDED
#define YOCTO_LW_ARRAYS_INCLUDED 1


#include "yocto/sequence/lw-array.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/records.hpp"

namespace yocto
{
	
	template <typename T,
	typename MEMORY_KIND>
	class lw_arrays 
	{
	public:
		typedef          lw_array<T>                     array_type;
		typedef typename type_traits<T>::mutable_type    mutable_type;
		
		explicit lw_arrays( size_t num_arrays ) : 
		arrays_(num_arrays),
		size_(0),
		wksp_(NULL),
		wlen_(0),
		indx_(0)
		{
			update();
		}
		
		virtual ~lw_arrays() throw()
		{
			memory::kind<MEMORY_KIND>:: template release_as<mutable_type>( wksp_, wlen_ );
			size_ = 0;
			update();
		}
		
		void prepare( const size_t new_size )
		{
			if( new_size != size_ )
			{
				size_t        new_wlen = new_size * num_arrays();
				mutable_type *new_wksp = memory::kind<MEMORY_KIND>:: template acquire_as<mutable_type>( new_wlen );
				
				memory::kind<MEMORY_KIND>:: template release_as<mutable_type>( wksp_, wlen_ );
				wksp_ = new_wksp;
				wlen_ = new_wlen;
				size_ = new_size;
				update();
			}
		}
		
		inline array_type & next_array() throw() { assert(indx_<num_arrays() ); return arrays_[indx_++]; }
		inline void         restart() throw() { indx_ = 0; }
		
		inline array_type & operator[]( size_t index ) throw()
		{
			assert( index > 0 ); 
			assert( index <= num_arrays() );
			return arrays_[index-1];
		}
		
		inline const array_type & operator[]( size_t index ) const throw()
		{
			assert( index > 0 ); 
			assert( index <= num_arrays() );
			return arrays_[index-1];
		}
		
		
		
		inline size_t common_size() const throw() { return size_;        }
		inline size_t num_arrays()  const throw() { return arrays_.size; }
		
		
	private:
		typedef memory::buffer_of<array_type,MEMORY_KIND> buffer_of_array;
		buffer_of_array   arrays_; //!< memory for arrays location
		size_t            size_;   //!< the common size
		mutable_type     *wksp_;   //!< dedicated workspace
		size_t            wlen_;   //!< workspace length
		size_t            indx_;   //!< dispatch index
		YOCTO_DISABLE_COPY_AND_ASSIGN(lw_arrays);
		
		inline void update() throw() 
		{
			mutable_type *p = wksp_;
			for( size_t i=0; i < arrays_.size; ++i, p += size_)
			{
				new ( &arrays_[i] ) array_type(p,size_);
			}
		}
	};
	
}


#endif
