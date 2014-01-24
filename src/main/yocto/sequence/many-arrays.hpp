#ifndef YOCTO_MANY_ARRAYS_INCLUDED
#define YOCTO_MANY_ARRAYS_INCLUDED 1


#include "yocto/sequence/arrays-mgr.hpp"

namespace yocto
{
	
	template <typename T,
	typename MEMORY_KIND>
	class lw_arrays : public arrays_mgr<T,MEMORY_KIND>
	{
	public:
        typedef arrays_mgr<T,MEMORY_KIND>      mgr_type;
		typedef typename mgr_type::array_type  array_type;
		
		explicit lw_arrays( size_t na ) :
        mgr_type(),
        num( na > 0 ? na : 1),
        arr( memory::kind<MEMORY_KIND>:: template acquire_as<array_type>(num) )
		{
			this->link();
		}
		
		virtual ~lw_arrays() throw()
		{
			this->release();
            memory::kind<MEMORY_KIND>:: template release_as<array_type>(arr,num);
		}
		
        
		virtual size_t num_arrays()  const throw() { return num; }
		
		
	private:
		size_t      num;
        array_type *arr;
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(lw_arrays);
		virtual void *arrays_addr() const throw() { return (void*)arr; }
		
	};
	
}


#endif
