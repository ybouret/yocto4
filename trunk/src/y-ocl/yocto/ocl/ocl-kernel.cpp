#include "yocto/ocl/kernel.hpp"
#include "yocto/ocl/_query.hpp"

//#include <iostream>

namespace yocto
{
	
	
	namespace ocl
	{
		
		namespace Core
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
				return __GetKernelWorkGroupInfo<size_t,CL_KERNEL_WORK_GROUP_SIZE>( kernel_, device, "CL_KERNEL_WORK_GROUP_SIZE" );
			}
			
			cl_ulong Kernel:: LOCAL_MEM_SIZE( cl_device_id device ) const
			{
				return __GetKernelWorkGroupInfo<cl_ulong,CL_KERNEL_LOCAL_MEM_SIZE>( kernel_, device, "CL_KERNEL_LOCAL_MEM_SIZE" );
				
			}
			
			
			Kernel:: ~ Kernel() throw()
			{
			}
			
			
#			define YOCTO_CLK_(FIELD,OF) FIELD( GetInfoValue( type2type<OF>(), clGetKernelInfo, kernel, CL_KERNEL_##FIELD, "CL_KERNEL_" #FIELD ) )
			
			Kernel:: Kernel( cl_kernel kernel ) :
			FUNCTION_NAME( GetInfoString( clGetKernelInfo, kernel, CL_KERNEL_FUNCTION_NAME, "CL_KERNEL_FUNCTION_NAME") ),
			YOCTO_CLK_(NUM_ARGS,cl_uint),
			YOCTO_CLK_(CONTEXT,cl_context),
			YOCTO_CLK_(PROGRAM,cl_program),
			kernel_( kernel )
			{
			}
			
			
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
			
			static inline cl_kernel __create_kernel( cl_program program, const char *function_name )
			{
				cl_int    err = CL_SUCCESS; 
				cl_kernel ker = clCreateKernel(program, function_name, &err );
				if( CL_SUCCESS != err )
					throw Exception( err, "clCreateKernel(%s)", function_name );
				return ker;
			}
			
		}
		
		Kernel:: Kernel( const Program & __program, const string & __function_name ) :
		hKernel_( Core::__create_kernel( * __program.hProgram_, & __function_name[0] ) ),
		pKernel_( new Core::Kernel( * hKernel_) )
		{
		}
		
		Kernel:: Kernel( const Program & __program, const char * __function_name ) :
		hKernel_( Core::__create_kernel( * __program.hProgram_,  __function_name ) ),
		pKernel_( new Core::Kernel( * hKernel_) )
		{
		}
		
		
		Kernel:: ~Kernel() throw() {}
		
		Kernel:: Kernel( const Kernel &K ) throw() :
		hKernel_( K.hKernel_ ),
		pKernel_( K.pKernel_ )
		{}
				
		
		const Core::Kernel * Kernel:: operator->() const throw() { return & (*pKernel_); }
		
		void Kernel:: SetArg(cl_uint     arg_index,
							 size_t      arg_size,
							 const void *arg_value)
		{
			//const Inner::Kernel & K = *pKernel_;
			//std::cerr << "[@" << (void *) *hKernel_ << "]='" << K.FUNCTION_NAME << "' SetArg #" << arg_index << std::endl;
			//std::cerr << "SetArg " << arg_index << " of Kernel@" << (void *) *hKernel_ << std::endl;
			const cl_int err = clSetKernelArg( *hKernel_, arg_index, arg_size, arg_value );
			if( CL_SUCCESS != err )
				throw Exception( err, "clSetKernelArg(%u)", unsigned(arg_index) );
		}
		
		bool operator==( const Kernel &lhs, const Kernel &rhs ) throw()
		{
			return (* lhs.hKernel_)  == (* rhs.hKernel_);
		}
		
		bool operator!=( const Kernel &lhs, const Kernel &rhs ) throw()
		{
			return (* lhs.hKernel_) != (* rhs.hKernel_);
		}
		
		
		
	}
	
}
