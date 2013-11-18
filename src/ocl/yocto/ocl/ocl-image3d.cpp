#include "yocto/ocl/image3d.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		static inline
		cl_mem __CreateImage3D(cl_context context,
							   cl_mem_flags flags,
							   const cl_image_format *image_format,
							   size_t image_width,
							   size_t image_height,
							   size_t image_depth,
							   size_t image_row_pitch,
							   size_t image_slice_pitch,
							   void *host_ptr)
		{
			cl_int err = CL_SUCCESS;
			cl_mem img = clCreateImage3D(context, 
										 flags,
										 image_format, 
										 image_width, 
										 image_height, 
										 image_depth,
										 image_row_pitch,
										 image_slice_pitch,
										 host_ptr, 
										 &err);
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateImage3D(%u,%u,%u)", unsigned(image_width), unsigned(image_height), unsigned(image_depth) );
			return img;
		}
		
		
		Image3D:: Image3D(const Context     &  __context,
						  cl_mem_flags            flags,
						  const ImageFormat     & image_format,
						  size_t                  image_width,
						  size_t                  image_height,
						  size_t                  image_depth,
						  size_t                  image_row_pitch,
						  size_t                  image_slice_pitch,
						  void                  * host_ptr ) :
		Image(  __CreateImage3D( * __context, 
								flags,
								(const cl_image_format *)&image_format, 
								image_width, 
								image_height,
								image_depth,
								image_row_pitch,
								image_slice_pitch,
								host_ptr)  
			  )
		{
		}
		
		
		Image3D:: Image3D( const Image3D &other ) throw() :
		Image( other )
		{
		}
		
		Image3D:: ~Image3D() throw() {}
		
		
		
		
	}
	
}

