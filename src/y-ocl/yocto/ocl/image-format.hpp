#ifndef YOCTO_OPENCL_IMAGE_FORMAT_INCLUDED
#define YOCTO_OPENCL_IMAGE_FORMAT_INCLUDED 1

#include "yocto/ocl/context.hpp"


namespace yocto
{
	
	namespace ocl
	{
		struct  ImageFormat;
		typedef ArrayOf<ImageFormat>::Type ImageFormats;
		
		
		//! binary layout of cl_image_format
		struct ImageFormat
		{
			cl_channel_order image_channel_order;
			cl_channel_type  image_channel_data_type;
			
			static const char *ChannelOrderName( cl_channel_order order) throw();
			static const char *ChannelTypeName(  cl_channel_type  t )  throw();
			
			const char * channel_order_name() const throw();
			const char * channel_data_type_name() const throw();
			
			static ImageFormats Supported(   const Context &__context, cl_mem_flags flags, cl_mem_object_type image_type );
			static ImageFormats Supported2D( const Context &__context, cl_mem_flags flags );
			static ImageFormats Supported3D( const Context &__context, cl_mem_flags flags );
			
		};
		
		
		
	}
	
}

#endif
