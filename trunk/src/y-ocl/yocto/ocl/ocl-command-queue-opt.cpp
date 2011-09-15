#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/exception.hpp"
#include "yocto/ocl/driver.hpp"
#include "yocto/code/make-index.hpp"

namespace yocto
{
	namespace ocl
	{
		
		
		void CommandQueue:: OptimizeNDRangeKernel(Kernel        &kernel,
												  cl_uint        work_dim,
												  const size_t  *global_work_size,
												  size_t        *local_work_size )
		{
			static const char fn[] = "OptimizeNDRangeKernel";
			assert( work_dim > 0 );
			assert( global_work_size != NULL );
			
			if( local_work_size )
			{
				YOCTO_OPENCL;
				const Device &D = OpenCL[ DEVICE ];
				if( work_dim > D.MAX_WORK_ITEM_DIMENSIONS )
					throw Exception( CL_INVALID_VALUE, "%s(work_dim=%u)", fn, work_dim);
				
				//--------------------------------------------------------------
				//
				// check initial values: if local_work_size if 0,
				// then set to the max.
				// Compute the initial software work group size
				//
				//--------------------------------------------------------------
				size_t sw_work_group_size = 1;
				for( cl_uint i=0; i < work_dim; ++i )
				{
					const size_t gws = global_work_size[i];
					if( gws <= 0 )
						throw Exception( CL_INVALID_VALUE, "%s(global_work_size[%u]=0)", fn, work_dim);
					
					size_t       lws     = local_work_size[i];
					const size_t max_lws = D.MAX_WORK_ITEM_SIZES[i];
					
					//-- raw value
					if( lws <= 0 ) 
						lws = max_lws;
					else 
					{
						if( lws > max_lws ) lws  = max_lws;
					}
					
					//-- align
					while( lws > 1 && ( ( gws % lws ) != 0 ) ) --lws;
					local_work_size[i]  = lws;
					sw_work_group_size *= lws;
				}
				
				//--------------------------------------------------------------
				//
				// get the recommended work_group_size 
				//
				//--------------------------------------------------------------
				const size_t hw_work_group_size = kernel.WORK_GROUP_SIZE( DEVICE );
				
				ArrayOf<size_t>::Type index( work_dim );
				make_c_index( local_work_size, work_dim, index() );
				
				
				//--------------------------------------------------------------
				//-- recursively decrease work sizes
				//--------------------------------------------------------------
				size_t counter = 0;
				while( sw_work_group_size > hw_work_group_size )
				{
					//-- get index to be modified
					const size_t idx = ( (counter++) % work_dim );
					const size_t j   = index[ idx ];
					
					//-- modify  it
					size_t     & lws = local_work_size[j];
					const size_t gws = global_work_size[j];
					assert( lws >= 1 );
					//std::cerr << "\tdecreasing " << lws << std::endl;
					if( lws > 1 )
					{
						--lws;
						while( lws > 1 && ( 0 != ( gws % lws ) ) ) --lws;
					}
					
					//-- check current values
					sw_work_group_size = local_work_size[0];
					for( size_t i=1; i < work_dim; ++i ) 
						sw_work_group_size *= local_work_size[i];
				}
				
				
			}
		}
		
	}
	
}
