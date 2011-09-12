#ifndef YOCTO_OPENCL_MEM_OBJECT_INCLUDED
#define YOCTO_OPENCL_MEM_OBJECT_INCLUDED 1

#include "yocto/ocl/context.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			//! Memory Object content
			class MemObject : public Object
			{
			public:
				virtual ~MemObject() throw();
				explicit MemObject( cl_mem mem );
				
				
				const cl_mem_object_type TYPE;
				const cl_mem_flags       FLAGS;
				const size_t             SIZE;
				const cl_context         CONTEXT;
				const cl_mem             ASSOCIATED_MEMOBJECT;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN( MemObject );
			};
			
		}
		
		//! Memory Object Base Class
		class MemObject : public Class
		{
		public:
			virtual ~MemObject() throw();
			
			
			friend bool operator==( const MemObject &lhs, const MemObject &rhs ) throw();
			friend bool operator!=( const MemObject &lhs, const MemObject &rhs ) throw();
			
		protected:
			explicit MemObject( cl_mem mem );
			MemObject( const MemObject & ) throw(); //!< Shared Copy
			Core::Shared<cl_mem>     hMem_;
			
			YOCTO_DISABLE_ASSIGN( MemObject );
		};
		
	}
	
	
}



#endif

