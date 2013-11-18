#ifndef YOCTO_C_ARRAY_INCLUDED
#define YOCTO_C_ARRAY_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
	namespace hidden
	{
		extern const char c_array_name[];
	}
	
	template <typename T>
	class c_array : public lw_array<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		virtual ~c_array() throw() {}
		explicit c_array( T *addr, size_t num_items ) throw() :
		lw_array<T>( addr, num_items ) {}
		
		explicit c_array() throw() : lw_array<T>(NULL,0) {}
		
		virtual const char *name()     const throw() { return hidden::c_array_name; }
		
		void reset( T *addr, size_t num_items ) throw() 
		{
			this->item_            = (mutable_type *)addr-1;
			(size_t&)(this->size_) = num_items;
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(c_array);
	};
}

#endif
