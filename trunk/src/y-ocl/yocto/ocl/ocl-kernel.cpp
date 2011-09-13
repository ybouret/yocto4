#include "yocto/ocl/kernel.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		
		template < typename T, cl_kernel_work_group_info INFO>
		static inline T __GetKernelWorkGroupInfo( cl_kernel kernel, cl_device_id device, const char *tag ) 
		{
			assert( tag );
			T ans;
			const cl_int err = clGetKernelWorkGroupInfo(kernel, device, INFO, sizeof(T), &ans, NULL );
			if( CL_SUCCESS != err ) throw Exception( err, "clGetKernelWorkGroupInfo(%s)", tag );
			return ans;
		}
		
		size_t Kernel:: WORK_GROUP_SIZE( cl_device_id device ) const
		{
			return __GetKernelWorkGroupInfo<size_t,CL_KERNEL_WORK_GROUP_SIZE>( *(*this), device, "CL_KERNEL_WORK_GROUP_SIZE" );
		}
		
		cl_ulong Kernel:: LOCAL_MEM_SIZE( cl_device_id device ) const
		{
			return __GetKernelWorkGroupInfo<cl_ulong,CL_KERNEL_LOCAL_MEM_SIZE>( *(*this), device, "CL_KERNEL_LOCAL_MEM_SIZE" );
			
		}
		
		
		Kernel:: ~Kernel() throw() {}
		
		
		static inline
		size_t _ref_count( cl_kernel kernel  )
		{
			cl_uint      ans = 0;
			const cl_int err = clGetKernelInfo( kernel, CL_KERNEL_REFERENCE_COUNT, sizeof(ans), &ans, NULL);
			if( err != CL_SUCCESS )
			{
				throw Exception( err, "clGetKernelInfo( REF_COUNT )" );
			}
			return ans;
		}
		
		YOCTO_OPENCL_SHARED_IMPL(cl_kernel,clRetainKernel,clReleaseKernel, _ref_count);
		
		static inline 
		cl_kernel __create_kernel( cl_program  program, const char *kernel_name )
		{
			cl_int    err = CL_SUCCESS; 
			cl_kernel ker = clCreateKernel(program, kernel_name, &err );
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateKernel(%s)", kernel_name );
			return ker;
		}
		
#			define YOCTO_CLK_(FIELD,OF) FIELD( Core::GetInfoValue( type2type<OF>(), clGetKernelInfo, *(*this), CL_KERNEL_##FIELD, "CL_KERNEL_" #FIELD ) )
		
		Kernel:: Kernel( Program &program, const string &kernel_name ) :
		Shared<cl_kernel>( __create_kernel( *program, kernel_name.c_str() )  ),
		FUNCTION_NAME( Core::GetInfoString( clGetKernelInfo, *(*this), CL_KERNEL_FUNCTION_NAME, "CL_KERNEL_FUNCTION_NAME") ),
		YOCTO_CLK_(NUM_ARGS,cl_uint)
		{
			
		}
		
		
		void Kernel::SetArg(cl_uint     arg_index,
							size_t      arg_size,
							const void *arg_value)
		{
			const cl_int err = clSetKernelArg( *(*this), arg_index, arg_size, arg_value);
			if( CL_SUCCESS != err )
				throw Exception( err, "Kernel('%s'), Arg #%u", FUNCTION_NAME.c_str(), arg_index );
		}
		
		void Kernel:: SetArg( cl_uint arg_index, MemObject &mem )
		{
			cl_mem obj = *mem;
			SetArg( arg_index, sizeof(cl_mem), &obj );
		}
		
		void Kernel:: SetArg(cl_uint arg_index, size_t local_mem_size )
		{
			SetArg(arg_index, local_mem_size, NULL);
		}
	}
	
}
