#include "yocto/ocl/image-format.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	namespace ocl
	{
		
#define __IFCO(NAME) case NAME: return #NAME
		const char *ImageFormat:: ChannelOrderName( cl_channel_order order) throw()
		{
			switch( order )
			{
					__IFCO(CL_R);
					__IFCO(CL_A);
					__IFCO(CL_RG);
					__IFCO(CL_RA);
					__IFCO(CL_RGB);
					__IFCO(CL_RGBA);
					__IFCO(CL_BGRA);
					__IFCO(CL_ARGB);
					__IFCO(CL_INTENSITY);
					__IFCO(CL_LUMINANCE);
					
				default:
					break;
			}
			return "UNKNOWN_ORDER";
		}
		
#define __IFCT(NAME) case NAME: return #NAME
		
		const char *ImageFormat:: ChannelTypeName( cl_channel_type t) throw()
		{
			switch( t )
			{
					
					__IFCT(CL_SNORM_INT8);
					__IFCT(CL_SNORM_INT16);             
					__IFCT(CL_UNORM_INT8);
					__IFCT(CL_UNORM_INT16);
					__IFCT(CL_UNORM_SHORT_565);                       
					__IFCT(CL_UNORM_SHORT_555);
					__IFCT(CL_UNORM_INT_101010);
					__IFCT(CL_SIGNED_INT8);                            
					__IFCT(CL_SIGNED_INT16);                           
					__IFCT(CL_SIGNED_INT32);                          
					__IFCT(CL_UNSIGNED_INT8);                           
					__IFCT(CL_UNSIGNED_INT16);                          
					__IFCT(CL_UNSIGNED_INT32);                          
					__IFCT(CL_HALF_FLOAT);                               
					__IFCT(CL_FLOAT);                                   
					
				default:
					break;	
			}
			return "UNKNOWN_TYPE";
			
		}
		
		
		const char * ImageFormat:: channel_order_name() const throw()
		{
			return ChannelOrderName( image_channel_order );
		}
		
		const char * ImageFormat:: channel_data_type_name() const throw()
		{
			return ChannelTypeName( image_channel_data_type );
		}
		
		
		ImageFormats ImageFormat:: Supported(const Context &__context,
											 cl_mem_flags          flags,
											 cl_mem_object_type    image_type )
		{
			cl_uint num = 0;
			{
				const cl_int err = clGetSupportedImageFormats( * __context, 
															  flags,
															  image_type,
															  0, 
															  NULL,
															  &num );
				if( CL_SUCCESS != err )
					throw Exception( err, "clGetSupportedImageFormat(COUNT)" );
			}
			
			ImageFormats  fmt( num );
			{
				const cl_int err = clGetSupportedImageFormats(* __context,
															  flags,
															  image_type,
															  num, 
															  (cl_image_format *)fmt(0),
															  NULL );
				if( CL_SUCCESS != err )
					throw Exception( err, "clGetSupportedImageFormat(VALUES)" );	
			}
			
			return fmt;
		}
		
		ImageFormats ImageFormat:: Supported2D( const Context &__context, cl_mem_flags flags )
		{
			return Supported( __context, flags, CL_MEM_OBJECT_IMAGE2D );
		}
		
		ImageFormats ImageFormat:: Supported3D( const Context &__context, cl_mem_flags flags )
		{
			return Supported( __context, flags, CL_MEM_OBJECT_IMAGE3D );
		}
		
	}
	
	
}
