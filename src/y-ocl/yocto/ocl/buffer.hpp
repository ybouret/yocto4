#ifndef YOCTO_OPENCL_BUFFER_INCLUDED
#define YOCTO_OPENCL_BUFFER_INCLUDED 1

#include "yocto/ocl/mem-object.hpp"

namespace yocto
{
	
	
	namespace ocl
	{
		
		namespace Core 
		{
			
			class Buffer : public MemObject
			{
			public:
				explicit Buffer( cl_mem buf );
				virtual ~Buffer() throw();
				
				typedef Proxy<Buffer> Pointer;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Buffer);
			};
			
		}
		
		
		class Buffer : public MemObject
		{
		public:
			explicit Buffer(const Context  &context,
							cl_mem_flags    flags,
							size_t          size,
							void *          host_ptr);
			virtual ~Buffer() throw();
			
			Buffer( const Buffer & ) throw(); //!< shared copy
			
			const Core::Buffer * operator->() const throw();
			
		private:
			Core::Buffer::Pointer pBuf_;
			YOCTO_DISABLE_ASSIGN( Buffer );
			
		};
		
		//! OpenCL templated buffer
		template <typename T>
		class BufferOf : public Buffer
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			explicit BufferOf(const Context  &context,
							  cl_mem_flags    flags,
							  size_t          num_items,
							  T              *base) :
			Buffer( context, flags, num_items * sizeof(T), base )
			{
			}
			
			virtual ~BufferOf() throw() {}
			
			
			//! shared copy
			inline BufferOf( const BufferOf &buf ) throw() : Buffer( buf ) {}
			
			
		private:
			YOCTO_DISABLE_ASSIGN(BufferOf);
		};
		
		
	}
	
}

#endif
