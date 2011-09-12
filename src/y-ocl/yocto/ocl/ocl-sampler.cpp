#include "yocto/ocl/sampler.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	
	namespace ocl
	{
		
		namespace Core
		{
			
#define YCL_SAMPLER_(FIELD,OF) FIELD( GetInfoValue( type2type<OF>(), clGetSamplerInfo, sampler, CL_SAMPLER_##FIELD, "CL_SAMPLER_" #FIELD ) )
			
			static inline
			size_t _ref_count( cl_sampler sampler )
			{
				const cl_uint YCL_SAMPLER_(REFERENCE_COUNT,cl_uint);
				return REFERENCE_COUNT;
			}
			
			YOCTO_OPENCL_SHARED_IMPL(cl_sampler,clRetainSampler,clReleaseSampler, _ref_count);
			
			Sampler:: Sampler( cl_sampler sampler ) :
			YCL_SAMPLER_(CONTEXT,cl_context),
			YCL_SAMPLER_(ADDRESSING_MODE,cl_addressing_mode),
			YCL_SAMPLER_(FILTER_MODE,cl_filter_mode),
			YCL_SAMPLER_(NORMALIZED_COORDS,cl_bool)
			{
			}
			
			Sampler:: ~Sampler() throw()
			{
			}
			
			
		}
		
		
		Sampler:: ~Sampler() throw() {}
		
		
		static inline
		cl_sampler __CreateSampler(cl_context context,
								   cl_bool normalized_coords,
								   cl_addressing_mode addressing_mode,
								   cl_filter_mode filter_mode )
		{
			cl_int     err     = CL_SUCCESS;
			cl_sampler sampler = clCreateSampler(context, normalized_coords, addressing_mode, filter_mode, &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateSampler" );
			return sampler;
		}
		
		Sampler:: Sampler(const Context & __context,
						  cl_bool            normalized_coords,
						  cl_addressing_mode addressing_mode,
						  cl_filter_mode     filter_mode) :
		hSampler_( __CreateSampler( * __context, normalized_coords, addressing_mode, filter_mode ) ),
		pSampler_( new Core::Sampler( *hSampler_ ) )
		{
		}
		
		Sampler:: Sampler( const Sampler &other ) throw() :
		hSampler_( other.hSampler_ ),
		pSampler_( other.pSampler_ )
		{
		}
		
		
	}
}
