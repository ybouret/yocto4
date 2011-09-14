#include "yocto/utest/run.hpp"
#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/image2d.hpp"
#include "yocto/ocl/image3d.hpp"

using namespace yocto;

static inline void display_fmt( const ocl::ImageFormats &fmt, const char *msg )
{
	std::cerr << msg << std::endl;
	std::cerr << "#formats= " << fmt.size << std::endl; 
	for( size_t i=0; i < fmt.size; ++i )
	{
		const ocl::ImageFormat &f = fmt[i];
		std::cerr << f.channel_order_name() << " | " << f.channel_data_type_name() << std::endl;
	}
}

YOCTO_UNIT_TEST_IMPL(images)
{
	
	YOCTO_OPENCL;
	const ocl::Platform &platform = OpenCL.Platforms[0];
	ocl::Context         context( platform.devices );
	
	{
		ocl::ImageFormats fmt = ocl::ImageFormat::Supported2D( context, CL_MEM_READ_ONLY );
		display_fmt( fmt, "2D CL_MEM_READ_ONLY" );
	}
	
	{
		ocl::ImageFormats fmt = ocl::ImageFormat::Supported2D( context, CL_MEM_READ_WRITE );
		display_fmt( fmt, "2D CL_MEM_READ_WRITE" );
	}
	
	{
		ocl::ImageFormats fmt = ocl::ImageFormat::Supported3D( context, CL_MEM_READ_ONLY );
		display_fmt( fmt, "3D CL_MEM_READ_ONLY" );
	}
	
	{
		ocl::ImageFormats fmt = ocl::ImageFormat::Supported3D( context, CL_MEM_READ_WRITE );
		display_fmt( fmt, "3D CL_MEM_READ_WRITE" );
	}
	
	
	
}
YOCTO_UNIT_TEST_DONE()
