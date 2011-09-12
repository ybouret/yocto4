#include "yocto/ocl/image2d.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		static inline
		cl_mem __CreateImage2D(cl_context context,
							   cl_mem_flags flags,
							   const cl_image_format *image_format,
							   size_t image_width,
							   size_t image_height,
							   size_t image_row_pitch,
							   void *host_ptr)
		{
			cl_int err = CL_SUCCESS;
			cl_mem img = clCreateImage2D(context, 
										 flags,
										 image_format, 
										 image_width, 
										 image_height, 
										 image_row_pitch,
										 host_ptr,
										 &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateImage2D(%u,%u)", unsigned(image_width), unsigned(image_height) );
			return img;
		}
		
		
		Image2D:: Image2D(const Context     &   __context,
						  cl_mem_flags            flags,
						  const ImageFormat     & image_format,
						  size_t                  image_width,
						  size_t                  image_height,
						  size_t                  image_row_pitch,
						  void                  * host_ptr ) :
		Image(  __CreateImage2D( * __context, 
								flags,
								(cl_image_format *)&image_format, 
								image_width, 
								image_height,
								image_row_pitch,
								host_ptr)  
			  )
		{
		}
		
		
		
		Image2D:: Image2D( const Image2D &other ) throw() :
		Image( other )
		{
		}
		
		Image2D:: ~Image2D() throw() {}
	}
	
}
