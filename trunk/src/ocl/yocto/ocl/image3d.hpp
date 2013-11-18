#ifndef YOCTO_OPENCL_IMAGE3D_INCLUDED
#define YOCTO_OPENCL_IMAGE3D_INCLUDED 1

#include "yocto/ocl/image.hpp"

namespace yocto
{
	
	namespace ocl
	{
		class Image3D : public Image
		{
		public:
			explicit Image3D(const Context    &   __context,
							 cl_mem_flags            flags,
							 const ImageFormat &     image_format,
							 size_t                  image_width,
							 size_t                  image_height,
							 size_t                  image_depth,
							 size_t                  image_row_pitch,
							 size_t                  image_slice_pitch,
							 void                  * host_ptr );
			virtual ~Image3D() throw();
			Image3D( const Image3D &) throw(); //!< Shared Copy
			
			
		private:
			YOCTO_DISABLE_ASSIGN(Image3D);
		};
		
		
	}
	
}

#endif

