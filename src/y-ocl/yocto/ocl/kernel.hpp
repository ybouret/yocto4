#ifndef YOCTO_OPENCL_KERNEL_INCLUDED
#define YOCTO_OPENCL_KERNEL_INCLUDED  1

#include "yocto/ocl/program.hpp"
//#include "yocto/ocl/buffer.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		namespace  Core
		{
			class Kernel : public Object
			{
			public:
				
				
				explicit Kernel( cl_kernel kernel );
				virtual ~Kernel() throw();
				
				typedef Proxy<Kernel> Pointer;
				
				const string      FUNCTION_NAME;
				const size_t      NUM_ARGS;
				const cl_context  CONTEXT;
				const cl_program  PROGRAM;
				
				
				size_t   WORK_GROUP_SIZE( cl_device_id device ) const;
				cl_ulong LOCAL_MEM_SIZE( cl_device_id device ) const;
				
			private:
				const cl_kernel kernel_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(Kernel);
			};
			
		}
		
		class Buffer;
		class Image;
		class CommandQueue;
		
		class Kernel : public Class
		{
		public:
			
			explicit Kernel( const Program & __program, const string & __function_name );
			explicit Kernel( const Program & __program, const char   * __function_name );
			virtual ~Kernel() throw();
			
			Kernel( const Kernel & ) throw(); //!< shared copy
			const Core::Kernel * operator->() const throw();
			
			void SetArg(cl_uint     arg_index,
						size_t      arg_size,
						const void *arg_value);
			
			void SetArg( cl_uint arg_index, Buffer & __memobj );
			void SetArg( cl_uint arg_index, Image  & __memobj );
			
			
			friend bool operator==( const Kernel &lhs, const Kernel &rhs ) throw();
			friend bool operator!=( const Kernel &lhs, const Kernel &rhs ) throw();
			
			
		private:
			Core::Shared<cl_kernel> hKernel_;
			Core::Kernel::Pointer   pKernel_;
			friend class CommandQueue;
			YOCTO_DISABLE_ASSIGN(Kernel);
		};
		
		
	}
	
}


#endif
