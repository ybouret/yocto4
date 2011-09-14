#include "yocto/ocl/mem-object.hpp"
#include "yocto/ocl/_query.hpp"


namespace yocto
{
	namespace ocl
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
		
		
		
		MemObject:: ~MemObject() throw()
		{
		}
		
#define YCLM_GET(FIELD,OF) FIELD( Core::GetInfoValue( type2type<OF>(), clGetMemObjectInfo, mem, CL_MEM_##FIELD, "CL_MEM_" #FIELD ) )

		MemObject:: MemObject( cl_mem mem) :
		Shared<cl_mem>( mem ),
		YCLM_GET(TYPE,cl_mem_object_type),
		YCLM_GET(FLAGS,cl_mem_flags),
		YCLM_GET(SIZE,size_t)
		{
			
		}
		
		MemObject:: MemObject( const MemObject &other ) throw() :
		Shared<cl_mem>( other ),
		TYPE(  other.TYPE  ),
		FLAGS( other.FLAGS ),
		SIZE(  other.SIZE  )
		{
		}
		
	}
}
