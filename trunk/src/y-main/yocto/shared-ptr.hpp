#ifndef YOCTO_SHARED_PTR_INCLUDED
#define YOCTO_SHARED_PTR_INCLUDED 1


#include "yocto/type-traits.hpp"

namespace yocto
{
	namespace core
	{
		size_t *create_ref();
		void    delete_ref( size_t * & ) throw();
	}
	
	template <typename T>
	class shared_ptr
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		inline shared_ptr( T *p ) : pointee_( (mutable_type*)p ), nref_(NULL)
		{
			try { nref_ = core::create_ref(); } 
			catch(...) { delete p; throw; } 
			assert( 1==nref_ );
		}
		
		
	private:
		mutable_type *pointee_;
		size_t       *nref_;
	};
	
}

#endif
