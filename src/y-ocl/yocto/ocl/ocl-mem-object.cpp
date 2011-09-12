#include "yocto/ocl/mem-object.hpp"
#include "yocto/ocl/_query.hpp"
namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			static inline
			size_t _ref_count( cl_mem m )
			{
				cl_uint      ans = 0;
				const cl_int err = clGetMemObjectInfo( m, CL_MEM_REFERENCE_COUNT, sizeof(ans), &ans, NULL);
				if( err != CL_SUCCESS )
				{
					throw Exception( err, "clGetMemObjetInfo( REF_COUNT )" );
				}
				return ans;
			}
			YOCTO_OPENCL_SHARED_IMPL(cl_mem,clRetainMemObject,clReleaseMemObject,_ref_count);
			
			
			MemObject:: ~ MemObject() throw()
			{
			}
			
#define YOCTO_CLM_(FIELD,OF) FIELD( GetInfoValue( type2type<OF>(), clGetMemObjectInfo, mem, CL_MEM_##FIELD, "CL_MEM_" #FIELD ) )
			
			MemObject:: MemObject( cl_mem mem ) :
			YOCTO_CLM_(TYPE,cl_mem_object_type),
			YOCTO_CLM_(FLAGS,cl_mem_flags),
			YOCTO_CLM_(SIZE,size_t),
			YOCTO_CLM_(CONTEXT,cl_context),
			ASSOCIATED_MEMOBJECT( mem )
			{
			}
			
		}
		
		MemObject:: ~MemObject() throw() {}
		
		
		MemObject:: MemObject(  cl_mem mem ) :
		hMem_( mem )
		{
		}
		
		
		MemObject:: MemObject( const MemObject &M ) throw() :
		hMem_( M.hMem_ )
		{
		}
		
		
		
		bool operator==( const MemObject &lhs, const MemObject &rhs ) throw()
		{
			return (*lhs.hMem_) == (*rhs.hMem_);
		}
		
		bool operator!=( const MemObject &lhs, const MemObject &rhs ) throw()
		{
			return (*lhs.hMem_) != (*rhs.hMem_);
		}
		
		
	}
	
	
	
}



