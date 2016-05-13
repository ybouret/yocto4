#ifndef YOCTO_LW_ARRAY_INCLUDED
#define YOCTO_LW_ARRAY_INCLUDED 1


#include "yocto/sequence/array.hpp"

namespace yocto
{

	namespace hidden
	{
		extern const char lw_array_name[];
		void              lw_array_reserve( const char * );
	}
	
	//! converts a C-style [0:n-1] array into a C++ [1:n] array
	/**
		no resizing operations are allowed
	 */
	template <typename T>
	class lw_array : public array<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		explicit lw_array( T *addr, size_t num_items ) throw() :
		item_( ((mutable_type *)addr)-1 ),
		size_( num_items )
		{ 
			assert( !(addr==NULL&&num_items>0) );
		}
		
		virtual const char *name()     const throw() { return hidden::lw_array_name; }
		virtual size_t      size()     const throw() { return size_; }
		virtual size_t      capacity() const throw() { return size_; }
		
		virtual void free()    throw() { /* do nothing */ } 
		virtual void release() throw() { /* do nothing */ }
		virtual void reserve(size_t) { hidden::lw_array_reserve( this->name() ); }
		
		virtual ~lw_array() throw() { item_=NULL; (size_t&)size_ = 0; }
		
	protected:
		mutable_type *item_;
		const size_t  size_;
        
	private:
		virtual const_type *get_item() const throw() { return item_; }
		YOCTO_DISABLE_COPY_AND_ASSIGN(lw_array);
	};
	
	
	
}

#endif
