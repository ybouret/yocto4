#ifndef YOCTO_OCL_BUFFER_INCLUDED
#define YOCTO_OCL_BUFFER_INCLUDED 1

#include "yocto/ocl/mem-object.hpp"
#include "yocto/ocl/context.hpp"

namespace yocto
{
	namespace ocl
	{
		
		class Buffer : public MemObject
		{
		public:
			explicit Buffer(const Context  &ctx, 
							cl_mem_flags    flags,
							size_t          size,
							void           *host_ptr ); 
			
			virtual ~Buffer() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Buffer);
		};
		
		template <typename T>
		class BufferOf : public Buffer
		{
		public:
			inline explicit BufferOf(const Context &ctx,
									 cl_mem_flags   flags,
									 size_t         num_items,
									 T        *     host_ptr) :
			Buffer(ctx,flags,num_items*sizeof(T),host_ptr) 
			{
			}
			
			inline virtual ~BufferOf() throw() {}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(BufferOf);
		};
		
		
	}
	
}



#endif
