#ifndef YOCTO_C_ARRAY_INCLUDED
#define YOCTO_C_ARRAY_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
	namespace hidden
	{
		extern const char c_array_name[];
		void   c_array_reserve( const char * );
	}
	
	template <typename T>
	class c_array : public array<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		virtual ~c_array() throw() {}
		explicit c_array( T *addr, size_t num_items ) throw() :
		item_( (mutable_type *)addr-1 ),
		size_( num_items ) {}
		
		explicit c_array() throw() : item_(NULL), size_(0) {}
		
		virtual const char *name()     const throw() { return hidden::c_array_name; }
		virtual size_t      size()     const throw() { return size_; }
		virtual size_t      capacity() const throw() { return size_; }
		
		virtual void free()    throw() { /* do nothing */ } 
		virtual void release() throw() { /* do nothing */ }
		virtual void reserve(size_t) { hidden::c_array_reserve( this->name() ); }
		
		void reset( T *addr, size_t num_items ) throw() 
		{
			item_          = (mutable_type *)addr-1;
			(size_t&)size_ = num_items;
		}
		
	private:
		mutable_type *item_;
		const size_t  size_;
		virtual const_type *get_item() const throw() { return item_; }
		YOCTO_DISABLE_COPY_AND_ASSIGN(c_array);
	};
}

#endif
