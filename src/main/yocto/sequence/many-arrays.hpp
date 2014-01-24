#ifndef YOCTO_MANY_ARRAYS_INCLUDED
#define YOCTO_MANY_ARRAYS_INCLUDED 1


#include "yocto/sequence/multi-arrays.hpp"

namespace yocto
{
	
	template <typename T,
	typename MEMORY_KIND>
	class many_arrays : public multi_arrays<T,MEMORY_KIND>
	{
	public:
        typedef multi_arrays<T,MEMORY_KIND>     multi_type;
        typedef typename multi_type::array_type array_type;
		
		explicit many_arrays( size_t na ) :
        multi_type(),
        num( na > 0 ? na : 1),
        arr( memory::kind<MEMORY_KIND>:: template acquire_as<array_type>(num) )
		{
			this->link();
		}
		
		virtual ~many_arrays() throw()
		{
			this->release();
            memory::kind<MEMORY_KIND>:: template release_as<array_type>(arr,num);
		}
		
        
		virtual size_t num_arrays()  const throw() { return num; }
		
		
	private:
		size_t      num;
        array_type *arr;
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(many_arrays);
		virtual void *arrays_addr() const throw() { return (void*)arr; }
		
	};
	
}


#endif
