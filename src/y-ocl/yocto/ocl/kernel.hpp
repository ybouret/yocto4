#ifndef YOCTO_OCL_KERNEL_INCLUDED
#define YOCTO_OCL_KERNEL_INCLUDED 1


#include "yocto/ocl/program.hpp"
#include "yocto/ocl/mem-object.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		class Kernel : public Shared<cl_kernel>
		{
		public:
			explicit Kernel( Program &program, const string &kernel_name );
			virtual ~Kernel() throw();
			
			const string  FUNCTION_NAME;
			const cl_uint NUM_ARGS;
			
			size_t   WORK_GROUP_SIZE( cl_device_id device ) const;
			cl_ulong LOCAL_MEM_SIZE( cl_device_id device ) const;
			
			void SetArg(cl_uint arg_index, size_t arg_size, const void *arg_value);
			void SetArg(cl_uint arg_index, MemObject &mem );
			void SetArg(cl_uint arg_index, size_t local_mem_size );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Kernel);
		};
		
	}
	
}

#endif
