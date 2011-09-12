#ifndef YOCTO_OCL_SAMPLER_INCLUDED
#define YOCTO_OCL_SAMPLER_INCLUDED 1

#include "yocto/ocl/context.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			class Sampler : public Object
			{
			public:
				explicit Sampler( cl_sampler sampler );
				virtual ~Sampler() throw();
				
				const cl_context         CONTEXT;
				const cl_addressing_mode ADDRESSING_MODE;
				const cl_filter_mode     FILTER_MODE;
				const cl_bool            NORMALIZED_COORDS;
				
				typedef Proxy<Sampler> Pointer;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Sampler);
			};
		}
		
		class Sampler : public Class
		{
		public:
			virtual ~Sampler() throw();
			explicit Sampler(const Context & __context,
							 cl_bool            normalized_coords,
							 cl_addressing_mode addressing_mode,
							 cl_filter_mode     filter_mode);
			
			Sampler( const Sampler & ) throw();
			
		private:
			Core::Shared<cl_sampler> hSampler_;
			Core::Sampler::Pointer   pSampler_;
			YOCTO_DISABLE_ASSIGN(Sampler);
		};
		
		
	}
	
	
}

#endif
