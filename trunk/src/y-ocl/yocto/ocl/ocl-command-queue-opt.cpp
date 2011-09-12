#include "yocto/ocl/command-queue.hpp"
#include "yocto/ocl/exception.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/code/hsort.hpp"

//#include <iostream>

namespace yocto
{
	
	
	namespace ocl
	{
		
		
		void CommandQueue:: _OptimizeNDRangeKernel(Kernel              &K,
												   cl_uint             work_dim,
												   const size_t *      global_work_size,
												   size_t       *      local_work_size ) const
		{
			const Core::CommandQueue  &queue  = *pCommandQueue_;
			const cl_device_id         device = queue.DEVICE;
			const Device              &D      = Driver::instance() [ device ];
			if( work_dim <= 0 || work_dim > D->MAX_WORK_ITEM_DIMENSIONS )
			{
				throw Exception(CL_INVALID_WORK_DIMENSION, 
								"_OptimizeNDRangeKernel(%u, not in 1-%u)", 
								unsigned( work_dim ), 
								unsigned( D->MAX_WORK_ITEM_DIMENSIONS ) );
			}
			
			assert( global_work_size != NULL );
			assert( local_work_size  != NULL );
			
			//------------------------------------------------------------------
			//-- pass 1: correct local work_group_sizes 
			//-- and compute software work_group_size
			//------------------------------------------------------------------
			size_t sw_work_group_size = 1;
			for( size_t i=0; i < work_dim; ++i )
			{
				const size_t gws = global_work_size[i];
				if( gws <= 0 ) 
					throw exception( "zero global_work_size[%u]", unsigned( i ) );
				
				//-- check range
				size_t lws    = clamp<size_t>(1, local_work_size[i], D->MAX_WORK_ITEM_SIZES[i] );
				
				//-- check aligment
				while( lws > 1 && ( 0 != ( gws % lws ) ) ) --lws;
				local_work_size[i]  = lws;
				sw_work_group_size *= lws;
				
				
			}
			
			//------------------------------------------------------------------
			//-- pass 2: get currently recommanded hardware work_group_size
			//-- and make an index of local work sizes
			//------------------------------------------------------------------
			const size_t hw_work_group_size = K->WORK_GROUP_SIZE( device );
			
			//-- order sizes
			Core::Sizes index(work_dim);
			sort<size_t>::make_index( local_work_size, work_dim, index(0) );
			
			
			//-- recursively decrease work sizes
			size_t counter = 0;
			while( sw_work_group_size > hw_work_group_size )
			{
				//-- get index to be modifiex
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
