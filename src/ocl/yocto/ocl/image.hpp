#ifndef YOCTO_OPENCL_IMAGE_INCLUDED
#define YOCTO_OPENCL_IMAGE_INCLUDED 1

#include "yocto/ocl/mem-object.hpp"
#include "yocto/ocl/image-format.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		
		
		class Image : public MemObject
		{
		public:
			
			virtual ~Image() throw();
			
			
			const cl_image_format FORMAT;
			const size_t          ELEMENT_SIZE;
			const size_t          ROW_PITCH;
			const size_t          SLICE_PITCH;
			const size_t          WIDTH;
			const size_t          HEIGHT;
			const size_t          DEPTH;
			
			Image( const Image &other ) throw(); //!< shared copy
			
		protected:
			explicit Image( cl_mem img );
			
		private:
			YOCTO_DISABLE_ASSIGN(Image);
		};
		
		
		
		
	}
	
}

#endif
