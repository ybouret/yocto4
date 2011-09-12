#ifndef YOCTO_OPENCL_IMAGE2D_INCLUDED
#define YOCTO_OPENCL_IMAGE2D_INCLUDED 1

#include "yocto/ocl/image.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		class Image2D : public Image
		{
		public:
			explicit Image2D(const Context       & __context,
							 cl_mem_flags            flags,
							 const ImageFormat   & __image_format,
							 size_t                  image_width,
							 size_t                  image_height,
							 size_t                  image_row_pitch,
							 void                  * host_ptr );
			virtual ~Image2D() throw();
			Image2D( const Image2D &) throw(); //!< Shared Copy
			
			
		private:
			YOCTO_DISABLE_ASSIGN(Image2D);
		};
		
	}
	
}
#endif
