#ifndef YOCTO_OCL_MEM_OBJECT_INCLUDED
#define YOCTO_OCL_MEM_OBJECT_INCLUDED 1

#include "yocto/ocl/types.hpp"

namespace yocto
{

	namespace ocl 
	{
	
		class MemObject : public Shared<cl_mem>
		{
		public:
			virtual ~MemObject() throw();
			
			const cl_mem_object_type TYPE;
			const cl_mem_flags       FLAGS;
			const size_t             SIZE;
			MemObject( const MemObject &other) throw(); //!< shared copy

		protected:
			explicit MemObject( cl_mem );
			
		private:
			YOCTO_DISABLE_ASSIGN(MemObject);
		};
		
	}
	
}

#endif
